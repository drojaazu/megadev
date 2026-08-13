"""Unit tests for the convention lint.

Run: python3 -m unittest discover -s tools/check/tests
These need no toolchain - they are pure text analysis.
"""

from __future__ import annotations

import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))

from mdcheck import conventions as cv  # noqa: E402


class TestStripComments(unittest.TestCase):
    def test_line_comment_removed(self):
        got = cv.strip_comments("#define FOO 1  // trailing\n")
        self.assertEqual(got, [(1, "#define FOO 1")])

    def test_block_comment_on_one_line(self):
        got = cv.strip_comments("#define A 1 /* note */ \n")
        self.assertEqual(got, [(1, "#define A 1")])

    def test_multiline_block_comment_hides_code_like_text(self):
        src = "/* comment\n   typedef int x;\n*/\n#define OK 1\n"
        got = dict(cv.strip_comments(src))
        self.assertEqual(got[2], "")  # the typedef inside the comment is gone
        self.assertEqual(got[4], "#define OK 1")

    def test_line_numbers_preserved(self):
        got = cv.strip_comments("a\nb\nc\n")
        self.assertEqual([n for n, _ in got], [1, 2, 3])


class TestDefH(unittest.TestCase):
    def test_clean_file_passes(self):
        src = (
            "/**\n * @file good.def.h\n */\n"
            "#ifndef MEGADEV__GOOD_DEF_H\n#define MEGADEV__GOOD_DEF_H\n"
            "#define FOO 0x1234  // comment\n"
            "#define BAR \\\n  (1 << 2)\n"
            "#endif\n"
        )
        self.assertEqual(cv.check_def_h(src, "x"), [])

    def test_typedef_is_flagged(self):
        out = cv.check_def_h("#define A 1\ntypedef unsigned short u16;\n", "x")
        self.assertEqual(len(out), 1)
        self.assertIn("typedef", out[0].detail)

    def test_struct_is_flagged(self):
        out = cv.check_def_h("struct Thing { int a; };\n", "x")
        self.assertEqual(len(out), 1)

    def test_line_continuation_is_not_flagged(self):
        # The continuation line is part of the #define, not stray code.
        out = cv.check_def_h("#define M(x) \\\n  do_thing(x)\n", "x")
        self.assertEqual(out, [])


class TestGuard(unittest.TestCase):
    """INV-4: headers guard themselves with #pragma once (SPEC.md OD-3)."""

    def test_pragma_once_passes(self):
        self.assertEqual(cv.check_guard("#pragma once\nint x;\n", "types.h", "x"), [])

    def test_pragma_once_with_leading_comment_passes(self):
        src = "/**\n * @file types.h\n */\n\n#pragma once\n"
        self.assertEqual(cv.check_guard(src, "types.h", "x"), [])

    def test_ifndef_guard_is_flagged(self):
        src = "#ifndef MEGADEV__TYPES_H\n#define MEGADEV__TYPES_H\n#endif\n"
        out = cv.check_guard(src, "types.h", "x")
        self.assertEqual(len(out), 1)
        self.assertIn("use #pragma once", out[0].detail)
        self.assertIn("MEGADEV__TYPES_H", out[0].detail)

    def test_missing_guard_flagged(self):
        out = cv.check_guard("int x;\n", "types.h", "x")
        self.assertEqual(len(out), 1)
        self.assertIn("no include guard", out[0].detail)


class TestFileTag(unittest.TestCase):
    def test_matching_tag_passes(self):
        self.assertEqual(cv.check_file_tag(" * @file types.h\n", "types.h", "x"), [])

    def test_path_qualified_tag_passes(self):
        self.assertEqual(
            cv.check_file_tag(" * @file main/bios.h\n", "main/bios.h", "x"), []
        )

    def test_absent_tag_is_not_a_violation(self):
        self.assertEqual(cv.check_file_tag("no tags here\n", "types.h", "x"), [])

    def test_wrong_tag_flagged(self):
        out = cv.check_file_tag(" * @file mmd.h\n", "main/md_sys.h", "x")
        self.assertEqual(len(out), 1)


class TestMnemonics(unittest.TestCase):
    def test_mov_flagged(self):
        out = cv.check_mnemonics("\tmov.l %d0,%d1\n", "x")
        self.assertEqual(len(out), 1)
        self.assertIn("use move, not mov", out[0].detail)

    def test_mov_after_local_label_flagged(self):
        out = cv.check_mnemonics("0:mov.l (a0)+, (a1)+\n", "x")
        self.assertEqual(len(out), 1)

    def test_mov_inside_inline_asm_string_flagged(self):
        out = cv.check_mnemonics('  "\\tmov.l %1, %3 \\n"\n', "x")
        self.assertEqual(len(out), 1)

    def test_move_is_clean(self):
        self.assertEqual(cv.check_mnemonics("\tmove.l %d0,%d1\n", "x"), [])

    def test_dbf_flagged_as_synonym(self):
        out = cv.check_mnemonics("\tdbf %d0, 1b\n", "x")
        self.assertIn("use dbra, not dbf", out[0].detail)

    def test_gnu_pseudo_ops_are_never_flagged(self):
        # jbsr/jra pick the smallest encoding; rewriting them costs size and
        # cycles. They are house style, not a violation. See SPEC.md STYLE-2.
        for line in ("\tjbsr foo\n", "\tjra loop\n", "\tjne 1b\n", "\tjeq done\n"):
            self.assertEqual(cv.check_mnemonics(line, "x"), [], msg=line)

    def test_comment_lines_ignored(self):
        self.assertEqual(cv.check_mnemonics(" * mov.l is an alias\n", "x"), [])
        self.assertEqual(cv.check_mnemonics("// mov.l here\n", "x"), [])

    def test_substring_not_matched(self):
        # `remove.l` must not trip the `mov` rule.
        self.assertEqual(cv.check_mnemonics("\tremove.l thing\n", "x"), [])


if __name__ == "__main__":
    unittest.main()

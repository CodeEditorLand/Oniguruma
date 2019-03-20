/*
 * This program was generated by testconv.rb.
 */
#include "config.h"
#ifdef ONIG_ESCAPE_UCHAR_COLLISION
#undef ONIG_ESCAPE_UCHAR_COLLISION
#endif
#include <stdio.h>

#include "oniguruma.h"

#include <string.h>

#define SLEN(s)  strlen(s)

static int nsucc  = 0;
static int nfail  = 0;
static int nerror = 0;

static FILE* err_file;

static OnigRegion* region;

static void xx(char* pattern, char* str, int from, int to, int mem, int not,
               int error_no)
{
  int r;
  regex_t* reg;
  OnigErrorInfo einfo;

  r = onig_new(&reg, (UChar* )pattern, (UChar* )(pattern + SLEN(pattern)),
	       ONIG_OPTION_DEFAULT, ONIG_ENCODING_UTF8, ONIG_SYNTAX_DEFAULT, &einfo);
  if (r) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];

    if (error_no == 0) {
      onig_error_code_to_str((UChar* )s, r, &einfo);
      fprintf(err_file, "ERROR: %s  /%s/\n", s, pattern);
      nerror++;
    }
    else {
      if (r == error_no) {
        fprintf(stdout, "OK(ERROR): /%s/ %d\n", pattern, r);
        nsucc++;
      }
      else {
        fprintf(stdout, "FAIL(ERROR): /%s/ '%s', %d, %d\n", pattern, str,
                error_no, r);
        nfail++;
      }
    }

    return ;
  }

  r = onig_search(reg, (UChar* )str, (UChar* )(str + SLEN(str)),
		  (UChar* )str, (UChar* )(str + SLEN(str)),
		  region, ONIG_OPTION_NONE);
  if (r < ONIG_MISMATCH) {
    char s[ONIG_MAX_ERROR_MESSAGE_LEN];

    if (error_no == 0) {
      onig_error_code_to_str((UChar* )s, r);
      fprintf(err_file, "ERROR: %s  /%s/\n", s, pattern);
      nerror++;
    }
    else {
      if (r == error_no) {
        fprintf(stdout, "OK(ERROR): /%s/ '%s', %d\n", pattern, str, r);
        nsucc++;
      }
      else {
        fprintf(stdout, "FAIL ERROR NO: /%s/ '%s', %d, %d\n", pattern, str,
                error_no, r);
        nfail++;
      }
    }

    return ;
  }

  if (r == ONIG_MISMATCH) {
    if (not) {
      fprintf(stdout, "OK(N): /%s/ '%s'\n", pattern, str);
      nsucc++;
    }
    else {
      fprintf(stdout, "FAIL: /%s/ '%s'\n", pattern, str);
      nfail++;
    }
  }
  else {
    if (not) {
      fprintf(stdout, "FAIL(N): /%s/ '%s'\n", pattern, str);
      nfail++;
    }
    else {
      if (region->beg[mem] == from && region->end[mem] == to) {
        fprintf(stdout, "OK: /%s/ '%s'\n", pattern, str);
        nsucc++;
      }
      else {
        fprintf(stdout, "FAIL: /%s/ '%s' %d-%d : %d-%d\n", pattern, str,
	        from, to, region->beg[mem], region->end[mem]);
        nfail++;
      }
    }
  }
  onig_free(reg);
}

static void x2(char* pattern, char* str, int from, int to)
{
  xx(pattern, str, from, to, 0, 0, 0);
}

static void x3(char* pattern, char* str, int from, int to, int mem)
{
  xx(pattern, str, from, to, mem, 0, 0);
}

static void n(char* pattern, char* str)
{
  xx(pattern, str, 0, 0, 0, 1, 0);
}

static void e(char* pattern, char* str, int error_no)
{
  xx(pattern, str, 0, 0, 0, 0, error_no);
}

extern int main(int argc, char* argv[])
{
  static OnigEncoding use_encs[] = { ONIG_ENCODING_UTF8 };

  onig_initialize(use_encs, sizeof(use_encs)/sizeof(use_encs[0]));

  err_file = stdout;

  region = onig_region_new();

  x2("", "", 0, 0);
  x2("^", "", 0, 0);
  x2("^a", "\na", 1, 2);
  x2("$", "", 0, 0);
  x2("$\\O", "bb\n", 2, 3);
  x2("\\G", "", 0, 0);
  x2("\\A", "", 0, 0);
  x2("\\Z", "", 0, 0);
  x2("\\z", "", 0, 0);
  x2("^$", "", 0, 0);
  x2("\\ca", "\001", 0, 1);
  x2("\\C-b", "\002", 0, 1);
  x2("\\c\\\\", "\034", 0, 1);
  x2("q[\\c\\\\]", "q\034", 0, 2);
  x2("", "a", 0, 0);
  x2("a", "a", 0, 1);
  x2("\\x61", "a", 0, 1);
  x2("aa", "aa", 0, 2);
  x2("aaa", "aaa", 0, 3);
  x2("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 0, 35);
  x2("ab", "ab", 0, 2);
  x2("b", "ab", 1, 2);
  x2("bc", "abc", 1, 3);
  x2("(?i:#RET#)", "#INS##RET#", 5, 10);
  x2("\\17", "\017", 0, 1);
  x2("\\x1f", "\x1f", 0, 1);
  x2("a(?#....\\\\JJJJ)b", "ab", 0, 2);
  x2("(?x)  G (o O(?-x)oO) g L", "GoOoOgLe", 0, 7);
  x2(".", "a", 0, 1);
  n(".", "");
  x2("..", "ab", 0, 2);
  x2("\\w", "e", 0, 1);
  n("\\W", "e");
  x2("\\s", " ", 0, 1);
  x2("\\S", "b", 0, 1);
  x2("\\d", "4", 0, 1);
  n("\\D", "4");
  x2("\\b", "z ", 0, 0);
  x2("\\b", " z", 1, 1);
  x2("\\b", "  z ", 2, 2);
  x2("\\B", "zz ", 1, 1);
  x2("\\B", "z ", 2, 2);
  x2("\\B", " z", 0, 0);
  x2("[ab]", "b", 0, 1);
  n("[ab]", "c");
  x2("[a-z]", "t", 0, 1);
  n("[^a]", "a");
  x2("[^a]", "\n", 0, 1);
  x2("[]]", "]", 0, 1);
  n("[^]]", "]");
  x2("[\\^]+", "0^^1", 1, 3);
  x2("[b-]", "b", 0, 1);
  x2("[b-]", "-", 0, 1);
  x2("[\\w]", "z", 0, 1);
  n("[\\w]", " ");
  x2("[\\W]", "b$", 1, 2);
  x2("[\\d]", "5", 0, 1);
  n("[\\d]", "e");
  x2("[\\D]", "t", 0, 1);
  n("[\\D]", "3");
  x2("[\\s]", " ", 0, 1);
  n("[\\s]", "a");
  x2("[\\S]", "b", 0, 1);
  n("[\\S]", " ");
  x2("[\\w\\d]", "2", 0, 1);
  n("[\\w\\d]", " ");
  x2("[[:upper:]]", "B", 0, 1);
  x2("[*[:xdigit:]+]", "+", 0, 1);
  x2("[*[:xdigit:]+]", "GHIKK-9+*", 6, 7);
  x2("[*[:xdigit:]+]", "-@^+", 3, 4);
  n("[[:upper]]", "A");
  x2("[[:upper]]", ":", 0, 1);
  x2("[\\044-\\047]", "\046", 0, 1);
  x2("[\\x5a-\\x5c]", "\x5b", 0, 1);
  x2("[\\x6A-\\x6D]", "\x6c", 0, 1);
  n("[\\x6A-\\x6D]", "\x6E");
  n("^[0-9A-F]+ 0+ UNDEF ", "75F 00000000 SECT14A notype ()    External    | _rb_apply");
  x2("[\\[]", "[", 0, 1);
  x2("[\\]]", "]", 0, 1);
  x2("[&]", "&", 0, 1);
  x2("[[ab]]", "b", 0, 1);
  x2("[[ab]c]", "c", 0, 1);
  n("[[^a]]", "a");
  n("[^[a]]", "a");
  x2("[[ab]&&bc]", "b", 0, 1);
  n("[[ab]&&bc]", "a");
  n("[[ab]&&bc]", "c");
  x2("[a-z&&b-y&&c-x]", "w", 0, 1);
  n("[^a-z&&b-y&&c-x]", "w");
  x2("[[^a&&a]&&a-z]", "b", 0, 1);
  n("[[^a&&a]&&a-z]", "a");
  x2("[[^a-z&&bcdef]&&[^c-g]]", "h", 0, 1);
  n("[[^a-z&&bcdef]&&[^c-g]]", "c");
  x2("[^[^abc]&&[^cde]]", "c", 0, 1);
  x2("[^[^abc]&&[^cde]]", "e", 0, 1);
  n("[^[^abc]&&[^cde]]", "f");
  x2("[a-&&-a]", "-", 0, 1);
  n("[a\\-&&\\-a]", "&");
  n("\\wabc", " abc");
  x2("a\\Wbc", "a bc", 0, 4);
  x2("a.b.c", "aabbc", 0, 5);
  x2(".\\wb\\W..c", "abb bcc", 0, 7);
  x2("\\s\\wzzz", " zzzz", 0, 5);
  x2("aa.b", "aabb", 0, 4);
  n(".a", "ab");
  x2(".a", "aa", 0, 2);
  x2("^a", "a", 0, 1);
  x2("^a$", "a", 0, 1);
  x2("^\\w$", "a", 0, 1);
  n("^\\w$", " ");
  x2("^\\wab$", "zab", 0, 3);
  x2("^\\wabcdef$", "zabcdef", 0, 7);
  x2("^\\w...def$", "zabcdef", 0, 7);
  x2("\\w\\w\\s\\Waaa\\d", "aa  aaa4", 0, 8);
  x2("\\A\\Z", "", 0, 0);
  x2("\\Axyz", "xyz", 0, 3);
  x2("xyz\\Z", "xyz", 0, 3);
  x2("xyz\\z", "xyz", 0, 3);
  x2("a\\Z", "a", 0, 1);
  x2("\\Gaz", "az", 0, 2);
  n("\\Gz", "bza");
  n("az\\G", "az");
  n("az\\A", "az");
  n("a\\Az", "az");
  x2("\\^\\$", "^$", 0, 2);
  x2("^x?y", "xy", 0, 2);
  x2("^(x?y)", "xy", 0, 2);
  x2("\\w", "_", 0, 1);
  n("\\W", "_");
  x2("(?=z)z", "z", 0, 1);
  n("(?=z).", "a");
  x2("(?!z)a", "a", 0, 1);
  n("(?!z)a", "z");
  x2("(?i:a)", "a", 0, 1);
  x2("(?i:a)", "A", 0, 1);
  x2("(?i:A)", "a", 0, 1);
  x2("(?i:i)", "I", 0, 1);
  x2("(?i:I)", "i", 0, 1);
  x2("(?i:[A-Z])", "i", 0, 1);
  x2("(?i:[a-z])", "I", 0, 1);
  n("(?i:A)", "b");
  x2("(?i:ss)", "ss", 0, 2);
  x2("(?i:ss)", "Ss", 0, 2);
  x2("(?i:ss)", "SS", 0, 2);
  /* 0xc5,0xbf == 017F: # LATIN SMALL LETTER LONG S */
  x2("(?i:ss)", "\xc5\xbfS", 0, 3);
  x2("(?i:ss)", "s\xc5\xbf", 0, 3);
  /* 0xc3,0x9f == 00DF: # LATIN SMALL LETTER SHARP S */
  x2("(?i:ss)", "\xc3\x9f", 0, 2);
  /* 0xe1,0xba,0x9e == 1E9E # LATIN CAPITAL LETTER SHARP S */
  x2("(?i:ss)", "\xe1\xba\x9e", 0, 3);
  x2("(?i:xssy)", "xssy", 0, 4);
  x2("(?i:xssy)", "xSsy", 0, 4);
  x2("(?i:xssy)", "xSSy", 0, 4);
  x2("(?i:xssy)", "x\xc5\xbfSy", 0, 5);
  x2("(?i:xssy)", "xs\xc5\xbfy", 0, 5);
  x2("(?i:xssy)", "x\xc3\x9fy", 0, 4);
  x2("(?i:xssy)", "x\xe1\xba\x9ey", 0, 5);
  x2("(?i:\xc3\x9f)", "ss", 0, 2);
  x2("(?i:\xc3\x9f)", "SS", 0, 2);
  x2("(?i:[\xc3\x9f])", "ss", 0, 2);
  x2("(?i:[\xc3\x9f])", "SS", 0, 2);
  x2("(?i)(?<!ss)z", "qqz", 2, 3);
  x2("(?i:[A-Z])", "a", 0, 1);
  x2("(?i:[f-m])", "H", 0, 1);
  x2("(?i:[f-m])", "h", 0, 1);
  n("(?i:[f-m])", "e");
  x2("(?i:[A-c])", "D", 0, 1);
  n("(?i:[^a-z])", "A");
  n("(?i:[^a-z])", "a");
  x2("(?i:[!-k])", "Z", 0, 1);
  x2("(?i:[!-k])", "7", 0, 1);
  x2("(?i:[T-}])", "b", 0, 1);
  x2("(?i:[T-}])", "{", 0, 1);
  x2("(?i:\\?a)", "?A", 0, 2);
  x2("(?i:\\*A)", "*a", 0, 2);
  n(".", "\n");
  x2("(?m:.)", "\n", 0, 1);
  x2("(?m:a.)", "a\n", 0, 2);
  x2("(?m:.b)", "a\nb", 1, 3);
  x2(".*abc", "dddabdd\nddabc", 8, 13);
  x2(".+abc", "dddabdd\nddabcaa\naaaabc", 8, 13);
  x2("(?m:.*abc)", "dddabddabc", 0, 10);
  n("(?i)(?-i)a", "A");
  n("(?i)(?-i:a)", "A");
  x2("a?", "", 0, 0);
  x2("a?", "b", 0, 0);
  x2("a?", "a", 0, 1);
  x2("a*", "", 0, 0);
  x2("a*", "a", 0, 1);
  x2("a*", "aaa", 0, 3);
  x2("a*", "baaaa", 0, 0);
  n("a+", "");
  x2("a+", "a", 0, 1);
  x2("a+", "aaaa", 0, 4);
  x2("a+", "aabbb", 0, 2);
  x2("a+", "baaaa", 1, 5);
  x2(".?", "", 0, 0);
  x2(".?", "f", 0, 1);
  x2(".?", "\n", 0, 0);
  x2(".*", "", 0, 0);
  x2(".*", "abcde", 0, 5);
  x2(".+", "z", 0, 1);
  x2(".+", "zdswer\n", 0, 6);
  x2("(.*)a\\1f", "babfbac", 0, 4);
  x2("(.*)a\\1f", "bacbabf", 3, 7);
  x2("((.*)a\\2f)", "bacbabf", 3, 7);
  x2("(.*)a\\1f", "baczzzzzz\nbazz\nzzzzbabf", 19, 23);
  x2("a|b", "a", 0, 1);
  x2("a|b", "b", 0, 1);
  x2("|a", "a", 0, 0);
  x2("(|a)", "a", 0, 0);
  x2("ab|bc", "ab", 0, 2);
  x2("ab|bc", "bc", 0, 2);
  x2("z(?:ab|bc)", "zbc", 0, 3);
  x2("a(?:ab|bc)c", "aabc", 0, 4);
  x2("ab|(?:ac|az)", "az", 0, 2);
  x2("a|b|c", "dc", 1, 2);
  x2("a|b|cd|efg|h|ijk|lmn|o|pq|rstuvwx|yz", "pqr", 0, 2);
  n("a|b|cd|efg|h|ijk|lmn|o|pq|rstuvwx|yz", "mn");
  x2("a|^z", "ba", 1, 2);
  x2("a|^z", "za", 0, 1);
  x2("a|\\Gz", "bza", 2, 3);
  x2("a|\\Gz", "za", 0, 1);
  x2("a|\\Az", "bza", 2, 3);
  x2("a|\\Az", "za", 0, 1);
  x2("a|b\\Z", "ba", 1, 2);
  x2("a|b\\Z", "b", 0, 1);
  x2("a|b\\z", "ba", 1, 2);
  x2("a|b\\z", "b", 0, 1);
  x2("\\w|\\s", " ", 0, 1);
  n("\\w|\\w", " ");
  x2("\\w|%", "%", 0, 1);
  x2("\\w|[&$]", "&", 0, 1);
  x2("[b-d]|[^e-z]", "a", 0, 1);
  x2("(?:a|[c-f])|bz", "dz", 0, 1);
  x2("(?:a|[c-f])|bz", "bz", 0, 2);
  x2("abc|(?=zz)..f", "zzf", 0, 3);
  x2("abc|(?!zz)..f", "abf", 0, 3);
  x2("(?=za)..a|(?=zz)..a", "zza", 0, 3);
  n("(?>a|abd)c", "abdc");
  x2("(?>abd|a)c", "abdc", 0, 4);
  x2("a?|b", "a", 0, 1);
  x2("a?|b", "b", 0, 0);
  x2("a?|b", "", 0, 0);
  x2("a*|b", "aa", 0, 2);
  x2("a*|b*", "ba", 0, 0);
  x2("a*|b*", "ab", 0, 1);
  x2("a+|b*", "", 0, 0);
  x2("a+|b*", "bbb", 0, 3);
  x2("a+|b*", "abbb", 0, 1);
  n("a+|b+", "");
  x2("(a|b)?", "b", 0, 1);
  x2("(a|b)*", "ba", 0, 2);
  x2("(a|b)+", "bab", 0, 3);
  x2("(ab|ca)+", "caabbc", 0, 4);
  x2("(ab|ca)+", "aabca", 1, 5);
  x2("(ab|ca)+", "abzca", 0, 2);
  x2("(a|bab)+", "ababa", 0, 5);
  x2("(a|bab)+", "ba", 1, 2);
  x2("(a|bab)+", "baaaba", 1, 4);
  x2("(?:a|b)(?:a|b)", "ab", 0, 2);
  x2("(?:a*|b*)(?:a*|b*)", "aaabbb", 0, 3);
  x2("(?:a*|b*)(?:a+|b+)", "aaabbb", 0, 6);
  x2("(?:a+|b+){2}", "aaabbb", 0, 6);
  x2("h{0,}", "hhhh", 0, 4);
  x2("(?:a+|b+){1,2}", "aaabbb", 0, 6);
  n("ax{2}*a", "0axxxa1");
  n("a.{0,2}a", "0aXXXa0");
  n("a.{0,2}?a", "0aXXXa0");
  n("a.{0,2}?a", "0aXXXXa0");
  x2("^a{2,}?a$", "aaa", 0, 3);
  x2("^[a-z]{2,}?$", "aaa", 0, 3);
  x2("(?:a+|\\Ab*)cc", "cc", 0, 2);
  n("(?:a+|\\Ab*)cc", "abcc");
  x2("(?:^a+|b+)*c", "aabbbabc", 6, 8);
  x2("(?:^a+|b+)*c", "aabbbbc", 0, 7);
  x2("a|(?i)c", "C", 0, 1);
  x2("(?i)c|a", "C", 0, 1);
  x2("(?i)c|a", "A", 0, 1);
  x2("a(?i)b|c", "aB", 0, 2);
  x2("a(?i)b|c", "aC", 0, 2);
  n("a(?i)b|c", "AC");
  n("a(?:(?i)b)|c", "aC");
  x2("(?i:c)|a", "C", 0, 1);
  n("(?i:c)|a", "A");
  x2("[abc]?", "abc", 0, 1);
  x2("[abc]*", "abc", 0, 3);
  x2("[^abc]*", "abc", 0, 0);
  n("[^abc]+", "abc");
  x2("a?\?", "aaa", 0, 0);
  x2("ba?\?b", "bab", 0, 3);
  x2("a*?", "aaa", 0, 0);
  x2("ba*?", "baa", 0, 1);
  x2("ba*?b", "baab", 0, 4);
  x2("a+?", "aaa", 0, 1);
  x2("ba+?", "baa", 0, 2);
  x2("ba+?b", "baab", 0, 4);
  x2("(?:a?)?\?", "a", 0, 0);
  x2("(?:a?\?)?", "a", 0, 0);
  x2("(?:a?)+?", "aaa", 0, 1);
  x2("(?:a+)?\?", "aaa", 0, 0);
  x2("(?:a+)?\?b", "aaab", 0, 4);
  x2("(?:ab)?{2}", "", 0, 0);
  x2("(?:ab)?{2}", "ababa", 0, 4);
  x2("(?:ab)*{0}", "ababa", 0, 0);
  x2("(?:ab){3,}", "abababab", 0, 8);
  n("(?:ab){3,}", "abab");
  x2("(?:ab){2,4}", "ababab", 0, 6);
  x2("(?:ab){2,4}", "ababababab", 0, 8);
  x2("(?:ab){2,4}?", "ababababab", 0, 4);
  x2("(?:ab){,}", "ab{,}", 0, 5);
  x2("(?:abc)+?{2}", "abcabcabc", 0, 6);
  x2("(?:X*)(?i:xa)", "XXXa", 0, 4);
  x2("(d+)([^abc]z)", "dddz", 0, 4);
  x2("([^abc]*)([^abc]z)", "dddz", 0, 4);
  x2("(\\w+)(\\wz)", "dddz", 0, 4);
  x3("(a)", "a", 0, 1, 1);
  x3("(ab)", "ab", 0, 2, 1);
  x2("((ab))", "ab", 0, 2);
  x3("((ab))", "ab", 0, 2, 1);
  x3("((ab))", "ab", 0, 2, 2);
  x3("((((((((((((((((((((ab))))))))))))))))))))", "ab", 0, 2, 20);
  x3("(ab)(cd)", "abcd", 0, 2, 1);
  x3("(ab)(cd)", "abcd", 2, 4, 2);
  x3("()(a)bc(def)ghijk", "abcdefghijk", 3, 6, 3);
  x3("(()(a)bc(def)ghijk)", "abcdefghijk", 3, 6, 4);
  x2("(^a)", "a", 0, 1);
  x3("(a)|(a)", "ba", 1, 2, 1);
  x3("(^a)|(a)", "ba", 1, 2, 2);
  x3("(a?)", "aaa", 0, 1, 1);
  x3("(a*)", "aaa", 0, 3, 1);
  x3("(a*)", "", 0, 0, 1);
  x3("(a+)", "aaaaaaa", 0, 7, 1);
  x3("(a+|b*)", "bbbaa", 0, 3, 1);
  x3("(a+|b?)", "bbbaa", 0, 1, 1);
  x3("(abc)?", "abc", 0, 3, 1);
  x3("(abc)*", "abc", 0, 3, 1);
  x3("(abc)+", "abc", 0, 3, 1);
  x3("(xyz|abc)+", "abc", 0, 3, 1);
  x3("([xyz][abc]|abc)+", "abc", 0, 3, 1);
  x3("((?i:abc))", "AbC", 0, 3, 1);
  x2("(abc)(?i:\\1)", "abcABC", 0, 6);
  x3("((?m:a.c))", "a\nc", 0, 3, 1);
  x3("((?=az)a)", "azb", 0, 1, 1);
  x3("abc|(.abd)", "zabd", 0, 4, 1);
  x2("(?:abc)|(ABC)", "abc", 0, 3);
  x3("(?i:(abc))|(zzz)", "ABC", 0, 3, 1);
  x3("a*(.)", "aaaaz", 4, 5, 1);
  x3("a*?(.)", "aaaaz", 0, 1, 1);
  x3("a*?(c)", "aaaac", 4, 5, 1);
  x3("[bcd]a*(.)", "caaaaz", 5, 6, 1);
  x3("(\\Abb)cc", "bbcc", 0, 2, 1);
  n("(\\Abb)cc", "zbbcc");
  x3("(^bb)cc", "bbcc", 0, 2, 1);
  n("(^bb)cc", "zbbcc");
  x3("cc(bb$)", "ccbb", 2, 4, 1);
  n("cc(bb$)", "ccbbb");
  n("(\\1)", "");
  n("\\1(a)", "aa");
  n("(a(b)\\1)\\2+", "ababb");
  n("(?:(?:\\1|z)(a))+$", "zaa");
  x2("(?:(?:\\1|z)(a))+$", "zaaa", 0, 4);
  x2("(a)(?=\\1)", "aa", 0, 1);
  n("(a)$|\\1", "az");
  x2("(a)\\1", "aa", 0, 2);
  n("(a)\\1", "ab");
  x2("(a?)\\1", "aa", 0, 2);
  x2("(a?\?)\\1", "aa", 0, 0);
  x2("(a*)\\1", "aaaaa", 0, 4);
  x3("(a*)\\1", "aaaaa", 0, 2, 1);
  x2("a(b*)\\1", "abbbb", 0, 5);
  x2("a(b*)\\1", "ab", 0, 1);
  x2("(a*)(b*)\\1\\2", "aaabbaaabb", 0, 10);
  x2("(a*)(b*)\\2", "aaabbbb", 0, 7);
  x2("(((((((a*)b))))))c\\7", "aaabcaaa", 0, 8);
  x3("(((((((a*)b))))))c\\7", "aaabcaaa", 0, 3, 7);
  x2("(a)(b)(c)\\2\\1\\3", "abcbac", 0, 6);
  x2("([a-d])\\1", "cc", 0, 2);
  x2("(\\w\\d\\s)\\1", "f5 f5 ", 0, 6);
  n("(\\w\\d\\s)\\1", "f5 f5");
  x2("(who|[a-c]{3})\\1", "whowho", 0, 6);
  x2("...(who|[a-c]{3})\\1", "abcwhowho", 0, 9);
  x2("(who|[a-c]{3})\\1", "cbccbc", 0, 6);
  x2("(^a)\\1", "aa", 0, 2);
  n("(^a)\\1", "baa");
  n("(a$)\\1", "aa");
  n("(ab\\Z)\\1", "ab");
  x2("(a*\\Z)\\1", "a", 1, 1);
  x2(".(a*\\Z)\\1", "ba", 1, 2);
  x3("(.(abc)\\2)", "zabcabc", 0, 7, 1);
  x3("(.(..\\d.)\\2)", "z12341234", 0, 9, 1);
  x2("((?i:az))\\1", "AzAz", 0, 4);
  n("((?i:az))\\1", "Azaz");
  x2("(?<=a)b", "ab", 1, 2);
  n("(?<=a)b", "bb");
  x2("(?<=a|b)b", "bb", 1, 2);
  x2("(?<=a|bc)b", "bcb", 2, 3);
  x2("(?<=a|bc)b", "ab", 1, 2);
  x2("(?<=a|bc||defghij|klmnopq|r)z", "rz", 1, 2);
  x3("(?<=(abc))d", "abcd", 0, 3, 1);
  x2("(?<=(?i:abc))d", "ABCd", 3, 4);
  x2("(a)\\g<1>", "aa", 0, 2);
  x2("(?<!a)b", "cb", 1, 2);
  n("(?<!a)b", "ab");
  x2("(?<!a|bc)b", "bbb", 0, 1);
  n("(?<!a|bc)z", "bcz");
  x2("(?<name1>a)", "a", 0, 1);
  x2("(?<name_2>ab)\\g<name_2>", "abab", 0, 4);
  x2("(?<name_3>.zv.)\\k<name_3>", "azvbazvb", 0, 8);
  x2("(?<=\\g<ab>)|-\\zEND (?<ab>XyZ)", "XyZ", 3, 3);
  x2("(?<n>|a\\g<n>)+", "", 0, 0);
  x2("(?<n>|\\(\\g<n>\\))+$", "()(())", 0, 6);
  x3("\\g<n>(?<n>.){0}", "X", 0, 1, 1);
  x2("\\g<n>(abc|df(?<n>.YZ){2,8}){0}", "XYZ", 0, 3);
  x2("\\A(?<n>(a\\g<n>)|)\\z", "aaaa", 0, 4);
  x2("(?<n>|\\g<m>\\g<n>)\\z|\\zEND (?<m>a|(b)\\g<m>)", "bbbbabba", 0, 8);
  x2("(?<name1240>\\w+\\sx)a+\\k<name1240>", "  fg xaaaaaaaafg x", 2, 18);
  x3("(z)()()(?<_9>a)\\g<_9>", "zaa", 2, 3, 1);
  x2("(.)(((?<_>a)))\\k<_>", "zaa", 0, 3);
  x2("((?<name1>\\d)|(?<name2>\\w))(\\k<name1>|\\k<name2>)", "ff", 0, 2);
  x2("(?:(?<x>)|(?<x>efg))\\k<x>", "", 0, 0);
  x2("(?:(?<x>abc)|(?<x>efg))\\k<x>", "abcefgefg", 3, 9);
  n("(?:(?<x>abc)|(?<x>efg))\\k<x>", "abcefg");
  x2("(?:(?<n1>.)|(?<n1>..)|(?<n1>...)|(?<n1>....)|(?<n1>.....)|(?<n1>......)|(?<n1>.......)|(?<n1>........)|(?<n1>.........)|(?<n1>..........)|(?<n1>...........)|(?<n1>............)|(?<n1>.............)|(?<n1>..............))\\k<n1>$", "a-pyumpyum", 2, 10);
  x3("(?:(?<n1>.)|(?<n1>..)|(?<n1>...)|(?<n1>....)|(?<n1>.....)|(?<n1>......)|(?<n1>.......)|(?<n1>........)|(?<n1>.........)|(?<n1>..........)|(?<n1>...........)|(?<n1>............)|(?<n1>.............)|(?<n1>..............))\\k<n1>$", "xxxxabcdefghijklmnabcdefghijklmn", 4, 18, 14);
  x3("(?<name1>)(?<name2>)(?<name3>)(?<name4>)(?<name5>)(?<name6>)(?<name7>)(?<name8>)(?<name9>)(?<name10>)(?<name11>)(?<name12>)(?<name13>)(?<name14>)(?<name15>)(?<name16>aaa)(?<name17>)$", "aaa", 0, 3, 16);
  x2("(?<foo>a|\\(\\g<foo>\\))", "a", 0, 1);
  x2("(?<foo>a|\\(\\g<foo>\\))", "((((((a))))))", 0, 13);
  x3("(?<foo>a|\\(\\g<foo>\\))", "((((((((a))))))))", 0, 17, 1);
  x2("\\g<bar>|\\zEND(?<bar>.*abc$)", "abcxxxabc", 0, 9);
  x2("\\g<1>|\\zEND(.a.)", "bac", 0, 3);
  x3("\\g<_A>\\g<_A>|\\zEND(.a.)(?<_A>.b.)", "xbxyby", 3, 6, 1);
  x2("\\A(?:\\g<pon>|\\g<pan>|\\zEND  (?<pan>a|c\\g<pon>c)(?<pon>b|d\\g<pan>d))$", "cdcbcdc", 0, 7);
  x2("\\A(?<n>|a\\g<m>)\\z|\\zEND (?<m>\\g<n>)", "aaaa", 0, 4);
  x2("(?<n>(a|b\\g<n>c){3,5})", "baaaaca", 1, 5);
  x2("(?<n>(a|b\\g<n>c){3,5})", "baaaacaaaaa", 0, 10);
  x2("(?<pare>\\(([^\\(\\)]++|\\g<pare>)*+\\))", "((a))", 0, 5);
  x2("()*\\1", "", 0, 0);
  x2("(?:()|())*\\1\\2", "", 0, 0);
  x2("(?:a*|b*)*c", "abadc", 4, 5);
  x3("(?:\\1a|())*", "a", 0, 0, 1);
  x2("x((.)*)*x", "0x1x2x3", 1, 6);
  x2("x((.)*)*x(?i:\\1)\\Z", "0x1x2x1X2", 1, 9);
  x2("(?:()|()|()|()|()|())*\\2\\5", "", 0, 0);
  x2("(?:()|()|()|(x)|()|())*\\2b\\5", "b", 0, 1);
  x2("[0-9-a]", "-", 0, 1);   // PR#44
  n("[0-9-a]", ":");          // PR#44
  x3("(\\(((?:[^(]|\\g<1>)*)\\))", "(abc)(abc)", 1, 4, 2); // PR#43
  x2("\\o{101}", "A", 0, 1);
  x2("\\A(a|b\\g<1>c)\\k<1+3>\\z", "bbacca", 0, 6);
  n("\\A(a|b\\g<1>c)\\k<1+3>\\z", "bbaccb");
  x2("(?i)\\A(a|b\\g<1>c)\\k<1+2>\\z", "bBACcbac", 0, 8);
  x2("(?i)(?<X>aa)|(?<X>bb)\\k<X>", "BBbb", 0, 4);
  x2("(?:\\k'+1'B|(A)C)*", "ACAB", 0, 4); // relative backref by postitive number
  x2("\\g<+2>(abc)(ABC){0}", "ABCabc", 0, 6); // relative call by positive number
  x2("A\\g'0'|B()", "AAAAB", 0, 5);
  x3("(A\\g'0')|B", "AAAAB", 0, 5, 1);
  x2("(a*)(?(1))aa", "aaaaa", 0, 5);
  x2("(a*)(?(-1))aa", "aaaaa", 0, 5);
  x2("(?<name>aaa)(?('name'))aa", "aaaaa", 0, 5);
  x2("(a)(?(1)aa|bb)a", "aaaaa", 0, 4);
  x2("(?:aa|())(?(<1>)aa|bb)a", "aabba", 0, 5);
  x2("(?:aa|())(?('1')aa|bb|cc)a", "aacca", 0, 5);
  x3("(a*)(?(1)aa|a)b", "aaab", 0, 1, 1);
  n("(a)(?(1)a|b)c", "abc");
  x2("(a)(?(1)|)c", "ac", 0, 2);
  n("(?()aaa|bbb)", "bbb");
  x2("(a)(?(1+0)b|c)d", "abd", 0, 3);
  x2("(?:(?'name'a)|(?'name'b))(?('name')c|d)e", "ace", 0, 3);
  x2("(?:(?'name'a)|(?'name'b))(?('name')c|d)e", "bce", 0, 3);
  x2("\\R", "\r\n", 0, 2);
  x2("\\R", "\r", 0, 1);
  x2("\\R", "\n", 0, 1);
  x2("\\R", "\x0b", 0, 1);
  n("\\R\\n", "\r\n");
  x2("\\R", "\xc2\x85", 0, 2);
  x2("\\N", "a", 0, 1);
  n("\\N", "\n");
  n("(?m:\\N)", "\n");
  n("(?-m:\\N)", "\n");
  x2("\\O", "a", 0, 1);
  x2("\\O", "\n", 0, 1);
  x2("(?m:\\O)", "\n", 0, 1);
  x2("(?-m:\\O)", "\n", 0, 1);
  x2("\\K", "a", 0, 0);
  x2("a\\K", "a", 1, 1);
  x2("a\\Kb", "ab", 1, 2);
  x2("(a\\Kb|ac\\Kd)", "acd", 2, 3);
  x2("(a\\Kb|\\Kac\\K)*", "acababacab", 9, 10);
  x2("(?:()|())*\\1", "abc", 0, 0);
  x2("(?:()|())*\\2", "abc", 0, 0);
  x2("(?:()|()|())*\\3\\1", "abc", 0, 0);
  x2("(|(?:a(?:\\g'1')*))b|", "abc", 0, 2);
  x2("^(\"|)(.*)\\1$", "XX", 0, 2);
  x2("(abc|def|ghi|jkl|mno|pqr|stu){0,10}?\\z", "admno", 2, 5);
  x2("(abc|(def|ghi|jkl|mno|pqr){0,7}?){5}\\z", "adpqrpqrpqr", 2, 11); // cover OP_REPEAT_INC_NG_SG
  x2("(?!abc).*\\z", "abcde", 1, 5); // cover OP_PREC_READ_NOT_END
  x2("(.{2,})?", "abcde", 0, 5); // up coverage
  x2("((a|b|c|d|e|f|g|h|i|j|k|l|m|n)+)?", "abcde", 0, 5); // up coverage
  x2("((a|b|c|d|e|f|g|h|i|j|k|l|m|n){3,})?", "abcde", 0, 5); // up coverage
  x2("((?:a(?:b|c|d|e|f|g|h|i|j|k|l|m|n))+)?", "abacadae", 0, 8); // up coverage
  x2("((?:a(?:b|c|d|e|f|g|h|i|j|k|l|m|n))+?)?z", "abacadaez", 0, 9); // up coverage
  x2("\\A((a|b)\?\?)?z", "bz", 0, 2); // up coverage
  x2("((?<x>abc){0}a\\g<x>d)+", "aabcd", 0, 5); // up coverage
  x2("((?(abc)true|false))+", "false", 0, 5); // up coverage
  x2("((?i:abc)d)+", "abcdABCd", 0, 8); // up coverage
  x2("((?<!abc)def)+", "bcdef", 2, 5); // up coverage
  x2("(\\ba)+", "aaa", 0, 1); // up coverage
  x2("()(?<x>ab)(?(<x>)a|b)", "aba", 0, 3); // up coverage
  x2("(?<=a.b)c", "azbc", 3, 4); // up coverage
  n("(?<=(?:abcde){30})z", "abc"); // up coverage
  x2("(?<=(?(a)a|bb))z", "aaz", 2, 3); // up coverage
  x2("[a]*\\W", "aa@", 0, 3); // up coverage
  x2("[a]*[b]", "aab", 0, 3); // up coverage
  n("a*\\W", "aaa"); // up coverage
  n("(?W)a*\\W", "aaa"); // up coverage
  x2("(?<=ab(?<=ab))", "ab", 2, 2); // up coverage
  x2("(?<x>a)(?<x>b)(\\k<x>)+", "abbaab", 0, 6); // up coverage
  x2("()(\\1)(\\2)", "abc", 0, 0); // up coverage
  x2("((?(a)b|c))(\\1)", "abab", 0, 4); // up coverage
  x2("(?<x>$|b\\g<x>)", "bbb", 0, 3); // up coverage
  x2("(?<x>(?(a)a|b)|c\\g<x>)", "cccb", 0, 4); // up coverage
  x2("(a)(?(1)a*|b*)+", "aaaa", 0, 4); // up coverage
  x2("[[^abc]&&cde]*", "de", 0, 2); // up coverage
  n("(a){10}{10}", "aa"); // up coverage
  x2("(?:a?)+", "aa", 0, 2); // up coverage
  x2("(?:a?)*?", "a", 0, 0); // up coverage
  x2("(?:a*)*?", "a", 0, 0); // up coverage
  x2("(?:a+?)*", "a", 0, 1); // up coverage
  x2("\\h", "5", 0, 1); // up coverage
  x2("\\H", "z", 0, 1); // up coverage
  x2("[\\h]", "5", 0, 1); // up coverage
  x2("[\\H]", "z", 0, 1); // up coverage
  x2("[\\o{101}]", "A", 0, 1); // up coverage
  x2("[\\u0041]", "A", 0, 1); // up coverage

  x2("(?~)", "", 0, 0);
  x2("(?~)", "A", 0, 0);
  x2("aaaaa(?~)", "aaaaaaaaaa", 0, 5);
  x2("(?~(?:|aaa))", "aaa", 0, 0);
  x2("(?~aaa|)", "aaa", 0, 0);
  x2("a(?~(?~)).", "abcdefghijklmnopqrstuvwxyz", 0, 26); // !!!
  x2("/\\*(?~\\*/)\\*/", "/* */ */", 0, 5);
  x2("(?~\\w+)zzzzz", "zzzzz", 0, 5);
  x2("(?~\\w*)zzzzz", "zzzzz", 0, 5);
  x2("(?~A.C|B)", "ABC", 0, 0);
  x2("(?~XYZ|ABC)a", "ABCa", 1, 4);
  x2("(?~XYZ|ABC)a", "aABCa", 0, 1);
  x2("<[^>]*>(?~[<>])</[^>]*>", "<a>vvv</a>   <b>  </b>", 0, 10);
  x2("(?~ab)", "ccc\ndab", 0, 5);
  x2("(?m:(?~ab))", "ccc\ndab", 0, 5);
  x2("(?-m:(?~ab))", "ccc\ndab", 0, 5);
  x2("(?~abc)xyz", "xyz012345678901234567890123456789abc", 0, 3);

  // absent with expr
  x2("(?~|78|\\d*)", "123456789", 0, 6);
  x2("(?~|def|(?:abc|de|f){0,100})", "abcdedeabcfdefabc", 0, 11);
  x2("(?~|ab|.*)", "ccc\nddd", 0, 3);
  x2("(?~|ab|\\O*)", "ccc\ndab", 0, 5);
  x2("(?~|ab|\\O{2,10})", "ccc\ndab", 0, 5);
  x2("(?~|ab|\\O{1,10})", "ab", 1, 2);
  n("(?~|ab|\\O{2,10})", "ab");
  x2("(?~|abc|\\O{1,10})", "abc", 1, 3);
  x2("(?~|ab|\\O{5,10})|abc", "abc", 0, 3);
  x2("(?~|ab|\\O{1,10})", "cccccccccccab", 0, 10);
  x2("(?~|aaa|)", "aaa", 0, 0);
  x2("(?~||a*)", "aaaaaa", 0, 0);
  x2("(?~||a*?)", "aaaaaa", 0, 0);
  x2("(a)(?~|b|\\1)", "aaaaaa", 0, 2);
  x2("(a)(?~|bb|(?:a\\1)*)", "aaaaaa", 0, 5);
  x2("(b|c)(?~|abac|(?:a\\1)*)", "abababacabab", 1, 4);
  n("(?~|c|a*+)a", "aaaaa");
  x2("(?~|aaaaa|a*+)", "aaaaa", 0, 0);
  x2("(?~|aaaaaa|a*+)b", "aaaaaab", 1, 7);
  x2("(?~|abcd|(?>))", "zzzabcd", 0, 0);
  x2("(?~|abc|a*?)", "aaaabc", 0, 0);

  // absent range cutter
  x2("(?~|abc)a*", "aaaaaabc", 0, 5);
  x2("(?~|abc)a*z|aaaaaabc", "aaaaaabc", 0, 8);
  x2("(?~|aaaaaa)a*", "aaaaaa", 0, 0);
  x2("(?~|abc)aaaa|aaaabc", "aaaabc", 0, 6);
  x2("(?>(?~|abc))aaaa|aaaabc", "aaaabc", 0, 6);
  x2("(?~|)a", "a", 0, 1);
  n("(?~|a)a", "a");
  x2("(?~|a)(?~|)a", "a", 0, 1);
  x2("(?~|a).*(?~|)a", "bbbbbbbbbbbbbbbbbbbba", 0, 21);
  x2("(?~|abc).*(xyz|pqr)(?~|)abc", "aaaaxyzaaapqrabc", 0, 16);
  x2("(?~|abc).*(xyz|pqr)(?~|)abc", "aaaaxyzaaaabcpqrabc", 11, 19);
  n("\\A(?~|abc).*(xyz|pqrabc)(?~|)abc", "aaaaxyzaaaabcpqrabcabc");

  x2("", "あ", 0, 0);
  x2("あ", "あ", 0, 3);
  n("い", "あ");
  x2("うう", "うう", 0, 6);
  x2("あいう", "あいう", 0, 9);
  x2("こここここここここここここここここここここここここここここここここここ", "こここここここここここここここここここここここここここここここここここ", 0, 105);
  x2("あ", "いあ", 3, 6);
  x2("いう", "あいう", 3, 9);
  x2("\\xca\\xb8", "\xca\xb8", 0, 2);
  x2(".", "あ", 0, 3);
  x2("..", "かき", 0, 6);
  x2("\\w", "お", 0, 3);
  n("\\W", "あ");
  x2("[\\W]", "う$", 3, 4);
  x2("\\S", "そ", 0, 3);
  x2("\\S", "漢", 0, 3);
  x2("\\b", "気 ", 0, 0);
  x2("\\b", " ほ", 1, 1);
  x2("\\B", "せそ ", 3, 3);
  x2("\\B", "う ", 4, 4);
  x2("\\B", " い", 0, 0);
  x2("[たち]", "ち", 0, 3);
  n("[なに]", "ぬ");
  x2("[う-お]", "え", 0, 3);
  n("[^け]", "け");
  x2("[\\w]", "ね", 0, 3);
  n("[\\d]", "ふ");
  x2("[\\D]", "は", 0, 3);
  n("[\\s]", "く");
  x2("[\\S]", "へ", 0, 3);
  x2("[\\w\\d]", "よ", 0, 3);
  x2("[\\w\\d]", "   よ", 3, 6);
  n("\\w鬼車", " 鬼車");
  x2("鬼\\W車", "鬼 車", 0, 7);
  x2("あ.い.う", "ああいいう", 0, 15);
  x2(".\\wう\\W..ぞ", "えうう うぞぞ", 0, 19);
  x2("\\s\\wこここ", " ここここ", 0, 13);
  x2("ああ.け", "ああけけ", 0, 12);
  n(".い", "いえ");
  x2(".お", "おお", 0, 6);
  x2("^あ", "あ", 0, 3);
  x2("^む$", "む", 0, 3);
  x2("^\\w$", "に", 0, 3);
  x2("^\\wかきくけこ$", "zかきくけこ", 0, 16);
  x2("^\\w...うえお$", "zあいううえお", 0, 19);
  x2("\\w\\w\\s\\Wおおお\\d", "aお  おおお4", 0, 16);
  x2("\\Aたちつ", "たちつ", 0, 9);
  x2("むめも\\Z", "むめも", 0, 9);
  x2("かきく\\z", "かきく", 0, 9);
  x2("かきく\\Z", "かきく\n", 0, 9);
  x2("\\Gぽぴ", "ぽぴ", 0, 6);
  n("\\Gえ", "うえお");
  n("とて\\G", "とて");
  n("まみ\\A", "まみ");
  n("ま\\Aみ", "まみ");
  x2("(?=せ)せ", "せ", 0, 3);
  n("(?=う).", "い");
  x2("(?!う)か", "か", 0, 3);
  n("(?!と)あ", "と");
  x2("(?i:あ)", "あ", 0, 3);
  x2("(?i:ぶべ)", "ぶべ", 0, 6);
  n("(?i:い)", "う");
  x2("(?m:よ.)", "よ\n", 0, 4);
  x2("(?m:.め)", "ま\nめ", 3, 7);
  x2("あ?", "", 0, 0);
  x2("変?", "化", 0, 0);
  x2("変?", "変", 0, 3);
  x2("量*", "", 0, 0);
  x2("量*", "量", 0, 3);
  x2("子*", "子子子", 0, 9);
  x2("馬*", "鹿馬馬馬馬", 0, 0);
  n("山+", "");
  x2("河+", "河", 0, 3);
  x2("時+", "時時時時", 0, 12);
  x2("え+", "ええううう", 0, 6);
  x2("う+", "おうううう", 3, 15);
  x2(".?", "た", 0, 3);
  x2(".*", "ぱぴぷぺ", 0, 12);
  x2(".+", "ろ", 0, 3);
  x2(".+", "いうえか\n", 0, 12);
  x2("あ|い", "あ", 0, 3);
  x2("あ|い", "い", 0, 3);
  x2("あい|いう", "あい", 0, 6);
  x2("あい|いう", "いう", 0, 6);
  x2("を(?:かき|きく)", "をかき", 0, 9);
  x2("を(?:かき|きく)け", "をきくけ", 0, 12);
  x2("あい|(?:あう|あを)", "あを", 0, 6);
  x2("あ|い|う", "えう", 3, 6);
  x2("あ|い|うえ|おかき|く|けこさ|しすせ|そ|たち|つてとなに|ぬね", "しすせ", 0, 9);
  n("あ|い|うえ|おかき|く|けこさ|しすせ|そ|たち|つてとなに|ぬね", "すせ");
  x2("あ|^わ", "ぶあ", 3, 6);
  x2("あ|^を", "をあ", 0, 3);
  x2("鬼|\\G車", "け車鬼", 6, 9);
  x2("鬼|\\G車", "車鬼", 0, 3);
  x2("鬼|\\A車", "b車鬼", 4, 7);
  x2("鬼|\\A車", "車", 0, 3);
  x2("鬼|車\\Z", "車鬼", 3, 6);
  x2("鬼|車\\Z", "車", 0, 3);
  x2("鬼|車\\Z", "車\n", 0, 3);
  x2("鬼|車\\z", "車鬼", 3, 6);
  x2("鬼|車\\z", "車", 0, 3);
  x2("\\w|\\s", "お", 0, 3);
  x2("\\w|%", "%お", 0, 1);
  x2("\\w|[&$]", "う&", 0, 3);
  x2("[い-け]", "う", 0, 3);
  x2("[い-け]|[^か-こ]", "あ", 0, 3);
  x2("[い-け]|[^か-こ]", "か", 0, 3);
  x2("[^あ]", "\n", 0, 1);
  x2("(?:あ|[う-き])|いを", "うを", 0, 3);
  x2("(?:あ|[う-き])|いを", "いを", 0, 6);
  x2("あいう|(?=けけ)..ほ", "けけほ", 0, 9);
  x2("あいう|(?!けけ)..ほ", "あいほ", 0, 9);
  x2("(?=をあ)..あ|(?=をを)..あ", "ををあ", 0, 9);
  x2("(?<=あ|いう)い", "いうい", 6, 9);
  n("(?>あ|あいえ)う", "あいえう");
  x2("(?>あいえ|あ)う", "あいえう", 0, 12);
  x2("あ?|い", "あ", 0, 3);
  x2("あ?|い", "い", 0, 0);
  x2("あ?|い", "", 0, 0);
  x2("あ*|い", "ああ", 0, 6);
  x2("あ*|い*", "いあ", 0, 0);
  x2("あ*|い*", "あい", 0, 3);
  x2("[aあ]*|い*", "aあいいい", 0, 4);
  x2("あ+|い*", "", 0, 0);
  x2("あ+|い*", "いいい", 0, 9);
  x2("あ+|い*", "あいいい", 0, 3);
  x2("あ+|い*", "aあいいい", 0, 0);
  n("あ+|い+", "");
  x2("(あ|い)?", "い", 0, 3);
  x2("(あ|い)*", "いあ", 0, 6);
  x2("(あ|い)+", "いあい", 0, 9);
  x2("(あい|うあ)+", "うああいうえ", 0, 12);
  x2("(あい|うえ)+", "うああいうえ", 6, 18);
  x2("(あい|うあ)+", "ああいうあ", 3, 15);
  x2("(あい|うあ)+", "あいをうあ", 0, 6);
  x2("(あい|うあ)+", "$$zzzzあいをうあ", 6, 12);
  x2("(あ|いあい)+", "あいあいあ", 0, 15);
  x2("(あ|いあい)+", "いあ", 3, 6);
  x2("(あ|いあい)+", "いあああいあ", 3, 12);
  x2("(?:あ|い)(?:あ|い)", "あい", 0, 6);
  x2("(?:あ*|い*)(?:あ*|い*)", "あああいいい", 0, 9);
  x2("(?:あ*|い*)(?:あ+|い+)", "あああいいい", 0, 18);
  x2("(?:あ+|い+){2}", "あああいいい", 0, 18);
  x2("(?:あ+|い+){1,2}", "あああいいい", 0, 18);
  x2("(?:あ+|\\Aい*)うう", "うう", 0, 6);
  n("(?:あ+|\\Aい*)うう", "あいうう");
  x2("(?:^あ+|い+)*う", "ああいいいあいう", 18, 24);
  x2("(?:^あ+|い+)*う", "ああいいいいう", 0, 21);
  x2("う{0,}", "うううう", 0, 12);
  x2("あ|(?i)c", "C", 0, 1);
  x2("(?i)c|あ", "C", 0, 1);
  x2("(?i:あ)|a", "a", 0, 1);
  n("(?i:あ)|a", "A");
  x2("[あいう]?", "あいう", 0, 3);
  x2("[あいう]*", "あいう", 0, 9);
  x2("[^あいう]*", "あいう", 0, 0);
  n("[^あいう]+", "あいう");
  x2("あ?\?", "あああ", 0, 0);
  x2("いあ?\?い", "いあい", 0, 9);
  x2("あ*?", "あああ", 0, 0);
  x2("いあ*?", "いああ", 0, 3);
  x2("いあ*?い", "いああい", 0, 12);
  x2("あ+?", "あああ", 0, 3);
  x2("いあ+?", "いああ", 0, 6);
  x2("いあ+?い", "いああい", 0, 12);
  x2("(?:天?)?\?", "天", 0, 0);
  x2("(?:天?\?)?", "天", 0, 0);
  x2("(?:夢?)+?", "夢夢夢", 0, 3);
  x2("(?:風+)?\?", "風風風", 0, 0);
  x2("(?:雪+)?\?霜", "雪雪雪霜", 0, 12);
  x2("(?:あい)?{2}", "", 0, 0);
  x2("(?:鬼車)?{2}", "鬼車鬼車鬼", 0, 12);
  x2("(?:鬼車)*{0}", "鬼車鬼車鬼", 0, 0);
  x2("(?:鬼車){3,}", "鬼車鬼車鬼車鬼車", 0, 24);
  n("(?:鬼車){3,}", "鬼車鬼車");
  x2("(?:鬼車){2,4}", "鬼車鬼車鬼車", 0, 18);
  x2("(?:鬼車){2,4}", "鬼車鬼車鬼車鬼車鬼車", 0, 24);
  x2("(?:鬼車){2,4}?", "鬼車鬼車鬼車鬼車鬼車", 0, 12);
  x2("(?:鬼車){,}", "鬼車{,}", 0, 9);
  x2("(?:かきく)+?{2}", "かきくかきくかきく", 0, 18);
  x3("(火)", "火", 0, 3, 1);
  x3("(火水)", "火水", 0, 6, 1);
  x2("((時間))", "時間", 0, 6);
  x3("((風水))", "風水", 0, 6, 1);
  x3("((昨日))", "昨日", 0, 6, 2);
  x3("((((((((((((((((((((量子))))))))))))))))))))", "量子", 0, 6, 20);
  x3("(あい)(うえ)", "あいうえ", 0, 6, 1);
  x3("(あい)(うえ)", "あいうえ", 6, 12, 2);
  x3("()(あ)いう(えおか)きくけこ", "あいうえおかきくけこ", 9, 18, 3);
  x3("(()(あ)いう(えおか)きくけこ)", "あいうえおかきくけこ", 9, 18, 4);
  x3(".*(フォ)ン・マ(ン()シュタ)イン", "フォン・マンシュタイン", 15, 27, 2);
  x2("(^あ)", "あ", 0, 3);
  x3("(あ)|(あ)", "いあ", 3, 6, 1);
  x3("(^あ)|(あ)", "いあ", 3, 6, 2);
  x3("(あ?)", "あああ", 0, 3, 1);
  x3("(ま*)", "ままま", 0, 9, 1);
  x3("(と*)", "", 0, 0, 1);
  x3("(る+)", "るるるるるるる", 0, 21, 1);
  x3("(ふ+|へ*)", "ふふふへへ", 0, 9, 1);
  x3("(あ+|い?)", "いいいああ", 0, 3, 1);
  x3("(あいう)?", "あいう", 0, 9, 1);
  x3("(あいう)*", "あいう", 0, 9, 1);
  x3("(あいう)+", "あいう", 0, 9, 1);
  x3("(さしす|あいう)+", "あいう", 0, 9, 1);
  x3("([なにぬ][かきく]|かきく)+", "かきく", 0, 9, 1);
  x3("((?i:あいう))", "あいう", 0, 9, 1);
  x3("((?m:あ.う))", "あ\nう", 0, 7, 1);
  x3("((?=あん)あ)", "あんい", 0, 3, 1);
  x3("あいう|(.あいえ)", "んあいえ", 0, 12, 1);
  x3("あ*(.)", "ああああん", 12, 15, 1);
  x3("あ*?(.)", "ああああん", 0, 3, 1);
  x3("あ*?(ん)", "ああああん", 12, 15, 1);
  x3("[いうえ]あ*(.)", "えああああん", 15, 18, 1);
  x3("(\\Aいい)うう", "いいうう", 0, 6, 1);
  n("(\\Aいい)うう", "んいいうう");
  x3("(^いい)うう", "いいうう", 0, 6, 1);
  n("(^いい)うう", "んいいうう");
  x3("ろろ(るる$)", "ろろるる", 6, 12, 1);
  n("ろろ(るる$)", "ろろるるる");
  x2("(無)\\1", "無無", 0, 6);
  n("(無)\\1", "無武");
  x2("(空?)\\1", "空空", 0, 6);
  x2("(空?\?)\\1", "空空", 0, 0);
  x2("(空*)\\1", "空空空空空", 0, 12);
  x3("(空*)\\1", "空空空空空", 0, 6, 1);
  x2("あ(い*)\\1", "あいいいい", 0, 15);
  x2("あ(い*)\\1", "あい", 0, 3);
  x2("(あ*)(い*)\\1\\2", "あああいいあああいい", 0, 30);
  x2("(あ*)(い*)\\2", "あああいいいい", 0, 21);
  x3("(あ*)(い*)\\2", "あああいいいい", 9, 15, 2);
  x2("(((((((ぽ*)ぺ))))))ぴ\\7", "ぽぽぽぺぴぽぽぽ", 0, 24);
  x3("(((((((ぽ*)ぺ))))))ぴ\\7", "ぽぽぽぺぴぽぽぽ", 0, 9, 7);
  x2("(は)(ひ)(ふ)\\2\\1\\3", "はひふひはふ", 0, 18);
  x2("([き-け])\\1", "くく", 0, 6);
  x2("(\\w\\d\\s)\\1", "あ5 あ5 ", 0, 10);
  n("(\\w\\d\\s)\\1", "あ5 あ5");
  x2("(誰？|[あ-う]{3})\\1", "誰？誰？", 0, 12);
  x2("...(誰？|[あ-う]{3})\\1", "あaあ誰？誰？", 0, 19);
  x2("(誰？|[あ-う]{3})\\1", "ういうういう", 0, 18);
  x2("(^こ)\\1", "ここ", 0, 6);
  n("(^む)\\1", "めむむ");
  n("(あ$)\\1", "ああ");
  n("(あい\\Z)\\1", "あい");
  x2("(あ*\\Z)\\1", "あ", 3, 3);
  x2(".(あ*\\Z)\\1", "いあ", 3, 6);
  x3("(.(やいゆ)\\2)", "zやいゆやいゆ", 0, 19, 1);
  x3("(.(..\\d.)\\2)", "あ12341234", 0, 11, 1);
  x2("((?i:あvず))\\1", "あvずあvず", 0, 14);
  x2("(?<愚か>変|\\(\\g<愚か>\\))", "((((((変))))))", 0, 15);
  x2("\\A(?:\\g<阿_1>|\\g<云_2>|\\z終了  (?<阿_1>観|自\\g<云_2>自)(?<云_2>在|菩薩\\g<阿_1>菩薩))$", "菩薩自菩薩自在自菩薩自菩薩", 0, 39);
  x2("[[ひふ]]", "ふ", 0, 3);
  x2("[[いおう]か]", "か", 0, 3);
  n("[[^あ]]", "あ");
  n("[^[あ]]", "あ");
  x2("[^[^あ]]", "あ", 0, 3);
  x2("[[かきく]&&きく]", "く", 0, 3);
  n("[[かきく]&&きく]", "か");
  n("[[かきく]&&きく]", "け");
  x2("[あ-ん&&い-を&&う-ゑ]", "ゑ", 0, 3);
  n("[^あ-ん&&い-を&&う-ゑ]", "ゑ");
  x2("[[^あ&&あ]&&あ-ん]", "い", 0, 3);
  n("[[^あ&&あ]&&あ-ん]", "あ");
  x2("[[^あ-ん&&いうえお]&&[^う-か]]", "き", 0, 3);
  n("[[^あ-ん&&いうえお]&&[^う-か]]", "い");
  x2("[^[^あいう]&&[^うえお]]", "う", 0, 3);
  x2("[^[^あいう]&&[^うえお]]", "え", 0, 3);
  n("[^[^あいう]&&[^うえお]]", "か");
  x2("[あ-&&-あ]", "-", 0, 1);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]q-w]", "え", 0, 3);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "f", 0, 1);
  x2("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "g", 0, 1);
  n("[^[^a-zあいう]&&[^bcdefgうえお]g-w]", "2");
  x2("a<b>バージョンのダウンロード<\\/b>", "a<b>バージョンのダウンロード</b>", 0, 44);
  x2(".<b>バージョンのダウンロード<\\/b>", "a<b>バージョンのダウンロード</b>", 0, 44);
  x2("\\n?\\z", "こんにちは", 15, 15);
  x2("(?m).*", "青赤黄", 0, 9);
  x2("(?m).*a", "青赤黄a", 0, 10);

  x2("\\p{Hiragana}", "ぴ", 0, 3);
  n("\\P{Hiragana}", "ぴ");
  x2("\\p{Emoji}", "\xE2\xAD\x90", 0, 3);
  x2("\\p{^Emoji}", "\xEF\xBC\x93", 0, 3);
  x2("\\p{Extended_Pictographic}", "\xE2\x9A\xA1", 0, 3);
  n("\\p{Extended_Pictographic}", "\xE3\x81\x82");

  x2("\\p{Word}", "こ", 0, 3);
  n("\\p{^Word}", "こ");
  x2("[\\p{Word}]", "こ", 0, 3);
  n("[\\p{^Word}]", "こ");
  n("[^\\p{Word}]", "こ");
  x2("[^\\p{^Word}]", "こ", 0, 3);
  x2("[^\\p{^Word}&&\\p{ASCII}]", "こ", 0, 3);
  x2("[^\\p{^Word}&&\\p{ASCII}]", "a", 0, 1);
  n("[^\\p{^Word}&&\\p{ASCII}]", "#");
  x2("[^[\\p{^Word}]&&[\\p{ASCII}]]", "こ", 0, 3);
  x2("[^[\\p{ASCII}]&&[^\\p{Word}]]", "こ", 0, 3);
  n("[[\\p{ASCII}]&&[^\\p{Word}]]", "こ");
  x2("[^[\\p{^Word}]&&[^\\p{ASCII}]]", "こ", 0, 3);
  x2("[^\\x{104a}]", "こ", 0, 3);
  x2("[^\\p{^Word}&&[^\\x{104a}]]", "こ", 0, 3);
  x2("[^[\\p{^Word}]&&[^\\x{104a}]]", "こ", 0, 3);
  n("[^\\p{Word}||[^\\x{104a}]]", "こ");

  x2("\\p{^Cntrl}", "こ", 0, 3);
  n("\\p{Cntrl}", "こ");
  x2("[\\p{^Cntrl}]", "こ", 0, 3);
  n("[\\p{Cntrl}]", "こ");
  n("[^\\p{^Cntrl}]", "こ");
  x2("[^\\p{Cntrl}]", "こ", 0, 3);
  x2("[^\\p{Cntrl}&&\\p{ASCII}]", "こ", 0, 3);
  x2("[^\\p{Cntrl}&&\\p{ASCII}]", "a", 0, 1);
  n("[^\\p{^Cntrl}&&\\p{ASCII}]", "#");
  x2("[^[\\p{^Cntrl}]&&[\\p{ASCII}]]", "こ", 0, 3);
  x2("[^[\\p{ASCII}]&&[^\\p{Cntrl}]]", "こ", 0, 3);
  n("[[\\p{ASCII}]&&[^\\p{Cntrl}]]", "こ");
  n("[^[\\p{^Cntrl}]&&[^\\p{ASCII}]]", "こ");
  n("[^\\p{^Cntrl}&&[^\\x{104a}]]", "こ");
  n("[^[\\p{^Cntrl}]&&[^\\x{104a}]]", "こ");
  n("[^\\p{Cntrl}||[^\\x{104a}]]", "こ");

  x2("(?-W:\\p{Word})", "こ", 0, 3);
  n("(?W:\\p{Word})", "こ");
  x2("(?W:\\p{Word})", "k", 0, 1);
  x2("(?-W:[[:word:]])", "こ", 0, 3);
  n("(?W:[[:word:]])", "こ");
  x2("(?-D:\\p{Digit})", "３", 0, 3);
  n("(?D:\\p{Digit})", "３");
  x2("(?-S:\\p{Space})", "\xc2\x85", 0, 2);
  n("(?S:\\p{Space})", "\xc2\x85");
  x2("(?-P:\\p{Word})", "こ", 0, 3);
  n("(?P:\\p{Word})", "こ");
  x2("(?-W:\\w)", "こ", 0, 3);
  n("(?W:\\w)", "こ");
  x2("(?-W:\\w)", "k", 0, 1);
  x2("(?W:\\w)", "k", 0, 1);
  n("(?-W:\\W)", "こ");
  x2("(?W:\\W)", "こ", 0, 3);
  n("(?-W:\\W)", "k");
  n("(?W:\\W)", "k");

  x2("(?-W:\\b)", "こ", 0, 0);
  n("(?W:\\b)", "こ");
  x2("(?-W:\\b)", "h", 0, 0);
  x2("(?W:\\b)", "h", 0, 0);
  n("(?-W:\\B)", "こ");
  x2("(?W:\\B)", "こ", 0, 0);
  n("(?-W:\\B)", "h");
  n("(?W:\\B)", "h");
  x2("(?-P:\\b)", "こ", 0, 0);
  n("(?P:\\b)", "こ");
  x2("(?-P:\\b)", "h", 0, 0);
  x2("(?P:\\b)", "h", 0, 0);
  n("(?-P:\\B)", "こ");
  x2("(?P:\\B)", "こ", 0, 0);
  n("(?-P:\\B)", "h");
  n("(?P:\\B)", "h");

  x2("\\p{InBasicLatin}", "\x41", 0, 1);
  //x2("\\p{Grapheme_Cluster_Break_Regional_Indicator}", "\xF0\x9F\x87\xA9", 0, 4);
  //n("\\p{Grapheme_Cluster_Break_Regional_Indicator}",  "\xF0\x9F\x87\xA5");

  // extended grapheme cluster

  // CR + LF
  n(".\\y\\O", "\x0d\x0a");
  x2(".\\Y\\O", "\x0d\x0a", 0, 2);

  // LATIN SMALL LETTER G, COMBINING DIAERESIS
  n("^.\\y.$", "\x67\xCC\x88");
  x2(".\\Y.", "\x67\xCC\x88", 0, 3);
  x2("\\y.\\Y.\\y", "\x67\xCC\x88", 0, 3);
  // HANGUL SYLLABLE GAG
  x2("\\y.\\y", "\xEA\xB0\x81", 0, 3);
  // HANGUL CHOSEONG KIYEOK, HANGUL JUNGSEONG A, HANGUL JONGSEONG KIYEOK
  x2("^.\\Y.\\Y.$", "\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8", 0, 9);
  n("^.\\y.\\Y.$", "\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8");
  // TAMIL LETTER NA, TAMIL VOWEL SIGN I,
  x2(".\\Y.", "\xE0\xAE\xA8\xE0\xAE\xBF", 0, 6);
  n(".\\y.", "\xE0\xAE\xA8\xE0\xAE\xBF");
  // THAI CHARACTER KO KAI, THAI CHARACTER SARA AM
  x2(".\\Y.", "\xE0\xB8\x81\xE0\xB8\xB3", 0, 6);
  n(".\\y.", "\xE0\xB8\x81\xE0\xB8\xB3");
  // DEVANAGARI LETTER SSA, DEVANAGARI VOWEL SIGN I
  x2(".\\Y.", "\xE0\xA4\xB7\xE0\xA4\xBF", 0, 6);
  n(".\\y.", "\xE0\xA4\xB7\xE0\xA4\xBF");

  // {Extended_Pictographic} Extend* ZWJ x {Extended_Pictographic}
  x2("..\\Y.", "\xE3\x80\xB0\xE2\x80\x8D\xE2\xAD\x95", 0, 9);
  x2("...\\Y.", "\xE3\x80\xB0\xCC\x82\xE2\x80\x8D\xE2\xAD\x95", 0, 11);
  n("...\\Y.", "\xE3\x80\xB0\xCD\xB0\xE2\x80\x8D\xE2\xAD\x95");

  // CR + LF
  n("^\\X\\X$", "\x0d\x0a");
  x2("^\\X$", "\x0d\x0a", 0, 2);
  // LATIN SMALL LETTER G, COMBINING DIAERESIS
  n("^\\X\\X.$", "\x67\xCC\x88");
  x2("^\\X$", "\x67\xCC\x88", 0, 3);
  // HANGUL CHOSEONG KIYEOK, HANGUL JUNGSEONG A, HANGUL JONGSEONG KIYEOK
  x2("^\\X$", "\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8", 0, 9);
  n("^\\X\\X\\X$", "\xE1\x84\x80\xE1\x85\xA1\xE1\x86\xA8");
  // TAMIL LETTER NA, TAMIL VOWEL SIGN I,
  x2("^\\X$", "\xE0\xAE\xA8\xE0\xAE\xBF", 0, 6);
  n("\\X\\X", "\xE0\xAE\xA8\xE0\xAE\xBF");
  // THAI CHARACTER KO KAI, THAI CHARACTER SARA AM
  x2("^\\X$", "\xE0\xB8\x81\xE0\xB8\xB3", 0, 6);
  n("\\X\\X", "\xE0\xB8\x81\xE0\xB8\xB3");
  // DEVANAGARI LETTER SSA, DEVANAGARI VOWEL SIGN I
  x2("^\\X$", "\xE0\xA4\xB7\xE0\xA4\xBF", 0, 6);
  n("\\X\\X", "\xE0\xA4\xB7\xE0\xA4\xBF");

  n("^\\X.$", "\xE0\xAE\xA8\xE0\xAE\xBF");

  // a + COMBINING GRAVE ACCENT (U+0300)
  x2("h\\Xllo", "ha\xCC\x80llo", 0, 7);

  x2("\\x40", "@", 0, 1);
  x2("\\x1", "\x01", 0, 1);
  x2("\\x{1}", "\x01", 0, 1);
  x2("\\x{4E38}", "\xE4\xB8\xB8", 0, 3);
  x2("\\u4E38", "\xE4\xB8\xB8", 0, 3);
  x2("\\u0040", "@", 0, 1);

  x2("c.*\\b", "abc", 2, 3);
  x2("\\b.*abc.*\\b", "abc", 0, 3);

  n("(*FAIL)", "abcdefg");
  n("abcd(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)(*FAIL)", "abcdefg");
  x2("(?:[ab]|(*MAX{2}).)*", "abcbaaccaaa", 0, 7);
  x2("(?:(*COUNT[AB]{X})[ab]|(*COUNT[CD]{X})[cd])*(*CMP{AB,<,CD})",
     "abababcdab", 5, 8);
  x2("(?(?{....})123|456)", "123", 0, 3);
  x2("(?(*FAIL)123|456)", "456", 0, 3);


  e("\\u040", "@", ONIGERR_INVALID_CODE_POINT_VALUE);
  e("(?<abc>\\g<abc>)", "zzzz", ONIGERR_NEVER_ENDING_RECURSION);
  e("(?<=(?>abc))", "abc", ONIGERR_INVALID_LOOK_BEHIND_PATTERN);
  e("(*FOO)", "abcdefg", ONIGERR_UNDEFINED_CALLOUT_NAME);

  fprintf(stdout,
       "\nRESULT   SUCC: %d,  FAIL: %d,  ERROR: %d      (by Oniguruma %s)\n",
       nsucc, nfail, nerror, onig_version());

  onig_region_free(region, 1);
  onig_end();

  return ((nfail == 0 && nerror == 0) ? 0 : -1);
}

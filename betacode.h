
int check_betacode(wchar_t *str);

/*
 * is_ascii_whitespace tests if a UTF-8 encoded string consists entirely
 * of the 7-bit ASCII whitespace characters.
 *
 * A null string will return 1 (true).
 * If it contains characters whose Unicode value is greater than 0x7f it will
 * return 0 (false), even if they are Unicode whitespace characters.
 *
 * The intended use case is when the caller is going to skip over strings that
 * are just whitespace, and do more extensive processing on strings that might
 * contain non-whitespace characters (but which might turn out, after UTF-8
 * decoding, to only contain Unicode whitespace).
 */

int is_ascii_whitespace(char *str);

#include "firestring_internal.h"

enum firestring_conf_parse_line_result strip_quotes(char **line) {
	char *s, *last_quote = NULL;
	int escaped = 0;
	if (line == NULL || *line == NULL)
		return FSC_MALFORMED;
	
	*line = firestring_chug(firestring_chomp(*line));
	if (**line != '"')
		return FSC_VALID;
	
	*line = *line + 1;
	for (s = *line; *s != '\0'; s++) {
		switch (*s) {
			case '\\':
				escaped = 1;
				memmove(s, s + 1, strlen(s) - 1);
				if (*s == '\0')
					s--;	
				else if (*s == '\n') {
					memmove(s, s + 1, strlen(s) - 1);	
					s[strlen(s) - 2] = '\0';
				}
				else if (s[0] == '\r' && s[1] == '\n') {
					memmove(s, s + 2, strlen(s) - 1);	
					s[strlen(s) - 3] = '\0';
				}
				else
					s[strlen(s) - 1] = '\0';
				break;
			case '"':
				if (!escaped)
					last_quote = s;
				escaped = 0;
				break;
			default:
				escaped = 0;
		}
	}
	if (last_quote == NULL)
		return FSC_INCOMPLETE;
	*last_quote = '\0';
	return FSC_VALID;
}


#ifndef _SPIDE_H
#define _SPIDE_H 1
#include "template.h"

struct spide_result_t {
	int	status;
	int 	http_status;
	int	content_len;
	char	*html;
};
typedef struct spide_result_t* SpideResultP;
typedef struct spide_result_t SpideResult;
typedef struct spide_result_t *SR;
#define SPIDE_SUCCESS	0
#define SPIDE_EDNS	1
#define SPIDE_ECONN	2
#define SPIDE_EHTTPSTATUS 3
SpideResultP spide(TemplateP template);
#endif

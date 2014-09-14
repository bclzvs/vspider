#ifndef _SPIDE_H
#define _SPIDE_H 1
#include "template.h"

struct spide_result_t {
	int http_status;
	char* html;
};
typedef struct spide_result_t* SpideResultP;
typedef struct spide_result_t SpideResult;

SpideResultP spide(TemplateP template);
#endif

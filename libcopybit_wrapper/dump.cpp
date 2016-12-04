#include <cutils/log.h>
#include "copybit.h"

void dump_image(const copybit_image_t *image, char *varname) {

    ALOGD("%s: w=%d h=%d format=%d horiz_padding=%d vert_padding=%d",
    varname,
    image->w,
    image->h,
    image->format,
    image->horiz_padding,
    image->vert_padding);

}

void dump_rect(const copybit_rect_t *rect, char *varname) {

    ALOGD("%s: l=%d t=%d r=%d b=%d",
    varname,
    rect->l,
    rect->t,
    rect->r,
    rect->b);

}

void dump_parameter(int name, int value) {

   char enumname[255];
   switch(name) {
        case COPYBIT_BLIT_TO_FRAMEBUFFER:
	    sprintf(enumname,"%s","COPYBIT_BLIT_TO_FRAMEBUFFER");
	    break;
        case COPYBIT_ROTATION_DEG:
	    sprintf(enumname,"%s","COPYBIT_ROTATION_DEG");
	    break;
        case COPYBIT_PLANE_ALPHA:
	    sprintf(enumname,"%s","COPYBIT_PLANE_ALPHA");
	    break;
        case COPYBIT_DITHER:
	    sprintf(enumname,"%s","COPYBIT_DITHER");
	    break;
        case COPYBIT_TRANSFORM:
	    sprintf(enumname,"%s","COPYBIT_TRANSFORM");
	    break;
        case COPYBIT_BLUR:
	    sprintf(enumname,"%s","COPYBIT_BLUR");
	    break;
        case COPYBIT_BLEND_MODE:
	    sprintf(enumname,"%s","COPYBIT_BLEND_MODE");
	    break;
        case COPYBIT_FRAMEBUFFER_WIDTH:
	    sprintf(enumname,"%s","COPYBIT_FRAMEBUFFER_WIDTH");
	    break;
        case COPYBIT_FRAMEBUFFER_HEIGHT:
	    sprintf(enumname,"%s","COPYBIT_FRAMEBUFFER_HEIGHT");
	    break;
        case COPYBIT_FG_LAYER:
	    sprintf(enumname,"%s","COPYBIT_FG_LAYER");
	    break;
        default:
            sprintf(enumname,"%s","UNKNOWN");
	    break;
   }
   ALOGD("%s -> %d",enumname,value);
}

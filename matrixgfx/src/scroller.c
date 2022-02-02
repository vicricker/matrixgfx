#include <assert.h>
#include <ctype.h>
#include <led-matrix-c.h>
#include <math.h>
#include <matrixgfx.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

int hexDigit(char c) {
  return (c >= '0' && c <= '9') ? c - '0' : 10 + toupper(c) - 'A';
}

/**
 * @brief Convert the two ASCII hex digits at src to an int.  If the digits aren't 0-F, expect strange results. GIGO
 * 
 * @param src 
 * @return int the value or -1 if two digits aren't available
 */
int readHex(char *src) {
  int result = -1;

  if (*src) {
    result = hexDigit(*src++) << 4;
  }

  if (*src) {
    result |= hexDigit(*src);
  } else {
    result = -1;
  }

  return result;
}

/**
 * @brief Set the attribute's color from the 6 hex digits at str if they exist
 * 
 * @param attrs 
 * @param str 
 * @return char* 
 */
char *setColor(struct Attributes *attrs, char *str) {
  int red = readHex(str);
  if (red >= 0) {
    attrs->color.r = red;
    str += 2;

    int green = readHex(str);
    if (green >= 0) {
      attrs->color.g = green;
      str += 2;

      int blue = readHex(str);
      if (blue >= 0) {
        attrs->color.b = readHex(str);
        str += 2;
      }
    }
  }

  return str;
}

/**
 * @brief Font indices are numbered 0-9
 *
 * @param scroller
 * @param attrs
 * @param msg
 * @return char* pointer to the char following the digit or the end of the
 * string.
 */
char *handleFont(struct Scroller *scroller, struct Attributes *attrs,
                 char *msg) {
  char f = *msg;

  if (isdigit(f)) {
    unsigned int font = f - '0';
    if (font < scroller->nfonts) {
      attrs->fontIndex = font;
    } else {
      fprintf(stderr, "Error: font %d > %d \n", font, scroller->nfonts);
    }
    msg++;
  }

  return msg;
}

/**
 * @brief Set the sine wave frequency (4 looks nice)
 *
 * Note: this could probably allow the amplitude to be set also.
 *
 * @param scroller
 * @param attrs
 * @param msg
 * @return char*
 */
char *handleSine(struct Scroller *scroller, struct Attributes *attrs,
                 char *msg) {
  char s = *msg;

  if (isdigit(s)) {
    attrs->sine = s - '0';
    msg++;
  }

  return msg;
}

/**
 * @brief Gradients are 1 based, with 0 being no gradient
 *
 * @param scroller
 * @param attrs
 * @param msg
 * @return char*
 */
char *handleGradient(struct Scroller *scroller, struct Attributes *attrs,
                     char *msg) {
  char g = *msg;
  if (isdigit(g)) {
    unsigned int gradient = g - '0';
    if (gradient <= scroller->ngradients) {
      attrs->gradient = gradient;
    } else {
      fprintf(stderr, "Error: gradient %d invalid\n", gradient);
    }
    msg++;
  }
}

/**
 * @brief Handle the escape sequences to set change the fonts, colors, etc.
 * 
 * See the FONT(), COLOR(), GRADIENT(), SINE() macros
 * 
 * @param scroller 
 * @param attrs 
 * @param msg 
 * @return char* 
 */
char *handleEscape(struct Scroller *scroller, struct Attributes *attrs,
                   char *msg) {
  char cmd = *msg;
  if (cmd) {
    msg++;
    switch (cmd) {
    case 'C':
      msg = setColor(attrs, msg);
      break;

    case 'F':
      msg = handleFont(scroller, attrs, msg);
      break;

    case 'S':
      msg = handleSine(scroller, attrs, msg);
      break;

    case 'G':
      msg = handleGradient(scroller, attrs, msg);
      break;
    }
  }
  return msg;
}

struct Attributes *createAttributes() {
  struct Attributes *attrs = calloc(1, sizeof(struct Attributes));

  attrs->color.r = 0x80;
  attrs->color.g = 0x80;
  attrs->color.b = 0x80;

  return attrs;
}

/**
 * @brief Parse the scroller message and evaluate the escape sequences
 * 
 * @param scroller 
 * @param msg 
 */
void parseMessage(struct Scroller *scroller, char *msg) {
  struct Attributes *attrs = createAttributes();

  int needNew = 0;
  int len = strlen(msg);

  char *txt = malloc(len);
  scroller->message = txt;

  struct Attributes **a = calloc(len + 1, sizeof(struct Attributes *));
  scroller->attributes = a;

  char c;
  while ((c = *msg++)) {
    if (c == ESC) {
      if (needNew) {
        struct Attributes *a = malloc(sizeof(struct Attributes));
        memcpy(a, attrs, sizeof(struct Attributes));
        attrs = a;
        needNew = 0;
      }

      msg = handleEscape(scroller, attrs, msg);
    } else {
      *txt++ = c;
      *a++ = attrs;
      needNew = 1;
    }
  }

  *a = NULL;
  *txt = 0;
}

/**
 * @brief Create a Scroller object
 * 
 * @param x Horizontal offset
 * @param fonts Array of font pointers.  There must be at least one.  (Can't draw text without a font!)
 * @param nfonts number of fonts
 * @param gradients Array of gradient pointers.  Can be null
 * @param ngradients Number of gradients
 * @param msg 
 * @return struct Scroller* 
 */
struct Scroller *createScroller(unsigned int x, unsigned int width, struct LedFont **fonts, int nfonts,
                                struct Gradient **gradients, int ngradients,
                                char *msg) {
  struct Scroller *scroller = calloc(1, sizeof(struct Scroller));

scroller->x=x;
  scroller->width = width;
  scroller->nfonts = nfonts;
  scroller->fonts = fonts;

  scroller->ngradients = ngradients;
  scroller->gradients = gradients;

  scroller->offset = 0;

  initTimer(&scroller->animationTimer, 20);

  parseMessage(scroller, msg);

  scroller->currentPos = 0;

  return scroller;
}

/**
 * @brief Advance the scroller (make it scroll to the left).  It uses the AnimationTimer for pacing.
 * 
 * @param scroller 
 */
void advanceScroller(struct Scroller *scroller) {
  if (animationReady(&scroller->animationTimer)) {
    // Shift left one px until character is offscreen
    scroller->currentPos -= 1;
    if (scroller->currentPos + scroller->currentWidth < scroller->x+1) {
      scroller->currentPos = scroller->x;

      scroller->offset++;

      if (!scroller->message[scroller->offset]) {
        scroller->offset = 0;
      }
    }
  }
}

/**
 * @brief Draw the scroller to the given canvas
 * 
 * @param canvas 
 * @param scroller 
 */
void drawScroller(struct LedCanvas *canvas, struct Scroller *scroller) {
  int pos = scroller->currentPos;

  char text[2];
  text[1] = 0;

  int o = scroller->offset;

  struct Color gradientColor = {0};

  while (1) {
    char c = scroller->message[o];

    if (c) {
      struct Attributes *attrs = scroller->attributes[o];
      struct LedFont *font = scroller->fonts[attrs->fontIndex];

      int y =
          (attrs->sine)
              ? scroller->y +
                    sin((double)(pos + o) * attrs->sine * 3.1415927 / 128) * 6
              : scroller->y;

      struct Color *color;
      if (!attrs->gradient) {
        color = &attrs->color;
      } else {
        float pct = (float)pos / scroller->width;
        if (pct < 0)
          pct = 0;

        gradientGetColor(scroller->gradients[attrs->gradient - 1], pct,
                         &gradientColor);
        color = &gradientColor;
      }

      text[0] = c;
      int w = draw_text(canvas, font, pos, y, color->r, color->g, color->b,
                        text, 0);

      if (pos <= scroller->x) {
        scroller->currentWidth = w;
      }

      pos += w;

      if (pos >= scroller->width) {
        break;
      }

      o++;
    } else {
      o = 0;
    }
  }
}

void freeScroller(struct Scroller *scroller) { free(scroller); }
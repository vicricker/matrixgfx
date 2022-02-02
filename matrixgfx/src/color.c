#include <matrixgfx.h>

int findComponent(int s, int e, float percent) { return s + percent * (e - s); }

void findColor(struct SetPoint *s, struct SetPoint *e, float percent,
               struct Color *color) {
  // Calculate the color between starting and ending SetPoints using the
  // given global percentage
  float lp = (percent - s->percent) / (e->percent - s->percent);

  color->r = findComponent(s->color.r, e->color.r, lp);
  color->g = findComponent(s->color.g, e->color.g, lp);
  color->b = findComponent(s->color.b, e->color.b, lp);
}

void gradientGetColor(struct Gradient *gradient, float percent,
                      struct Color *color) {
  int first=0;
  for (int i = gradient->nsetPoints - 1; i > -1; i--) {
    if (percent >= gradient->setPoints[i].percent) {
      first = i;
      break;
    }
  }

  if (first == gradient->nsetPoints - 1) {
    color->r = gradient->setPoints[first].color.r;
    color->g = gradient->setPoints[first].color.g;
    color->b = gradient->setPoints[first].color.b;
  } else {
    struct SetPoint *s = &gradient->setPoints[first];
    struct SetPoint *e = &gradient->setPoints[first + 1];

    findColor(s, e, percent, color);
  }
}

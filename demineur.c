#include "toolbox.h"
#define SCALE   33
#define X_MAX   30
#define Y_MAX   30

#define EMPTY   10
#define MINE    11
#define REDMINE 12
#define ERROR   13
#define WIN     14
#define FLAG    15
#define HINT    16
#define UNKNOW  17
#define SAFE    18
#define DANGER  19
#define OFF     20
#define MINUS   21
#define PLUS    22
#define X       23

typedef struct {
  int mine;
  int number;
  int display;
} CELLULE;

typedef struct {
  CELLULE cell[X_MAX][Y_MAX];
} GRILLE;

/*
// moved to graphics.h
typedef struct {
  POINT coord;
  int button;
} MOUSE;

MOUSE wait_mouse_click() {
  MOUSE P;
  in.mouse_click[0].x = in.mouse_click[0].y = -1;
  in.mouse_click[1].x = in.mouse_click[1].y = -1;
  in.mouse_click[2].x = in.mouse_click[2].y = -1;

  while(in.mouse_click[0].x == -1 && in.mouse_click[1].x == -1 && in.mouse_click[2].x == -1) {
    check_events();
    attendre(20);
  }

  if(in.mouse_click[0].x != -1) {
    P.coord = in.mouse_click[0];
    P.button = 0;
  } else if(in.mouse_click[1].x != -1) {
    P.coord = in.mouse_click[1];
    P.button = 1;
  } else {
    P.coord = in.mouse_click[2];
    P.button = 2;
  }

  in.mouse_click[0].x = in.mouse_click[0].y = -1;
  in.mouse_click[1].x = in.mouse_click[1].y = -1;
  in.mouse_click[2].x = in.mouse_click[2].y = -1;
  return P;
}

void resize_window(int W, int H) {
	SDL_Event event;

  if((W > 10) && (W < MAX_WIDTH)) WIDTH  = W;
  else WIDTH  = 600;

  if((H > 10) && (H < MAX_HEIGHT)) HEIGHT = H;
  else HEIGHT = 400;

  SDL_SetWindowSize(SDL_screen, W, H);

  SDL_Delay(10);

  remplir_ecran(noir);
  show_all();
}
/**/

int nb_x = 0;
int nb_y = 0;
int nb_mine = 0;
int nb_flag = 0;
int nb_flag_mine = 0;
int nb_hidden = 0;
int mode = 0;
int end = 0;
IMAGE imgs[32];
GRILLE grid;
MOUSE click;

// forked from toolbox.c to customize
int input_entier() {
  int n;
  fflush(stdout);
  scanf("%d", &n);
  while(getchar() != '\n'); // flush user input
  return n;
}

BOOL has_mine(int i, int j) {
  return (grid.cell[i][j].mine == 1);
}

BOOL in_bbox(int x_min, int x_max, int y_min, int y_max, int i, int j) {
  return (i >= x_min && i < x_max && j >= y_min && j < y_max);
}

BOOL is_display(int i, int j, int k) {
  return (grid.cell[i][j].display == k);
}

BOOL is_hide(int i, int j) {
  return (is_display(i, j, 0) || is_display(i, j, 4) || is_display(i, j, 5) ||
          is_display(i, j, 10) || is_display(i, j, 11));
}

void set_display(int i, int j, int k) {
  grid.cell[i][j].display = k;
}

void draw_cell(int x, int y, int img) {
  POINT p = {.x = x * SCALE, .y = y * SCALE};
  draw_img(imgs[img], p);
}

void draw_rect(int x_min, int x_max, int y_min, int y_max, int img) {
  int x, y;
  for(x = x_min; x <= x_max; x++)
    for(y = y_min; y <= y_max; y++) {
      draw_cell(x, y, img);
    }
}

void draw_number(int x, int y, int s, int n) {
  int i, p = 1;
  for(i = 0; i < s; i++) {
    draw_cell(x + s - 1 - i, y, (n / p) % 10);
    p = p * 10;
  }
}

void init() { //MODIF INPUT WINDOW
  imgs[0] = load_img("res/0.bmp");
  imgs[1] = load_img("res/1.bmp");
  imgs[2] = load_img("res/2.bmp");
  imgs[3] = load_img("res/3.bmp");
  imgs[4] = load_img("res/4.bmp");
  imgs[5] = load_img("res/5.bmp");
  imgs[6] = load_img("res/6.bmp");
  imgs[7] = load_img("res/7.bmp");
  imgs[8] = load_img("res/8.bmp");
  imgs[9] = load_img("res/9.bmp");
  imgs[EMPTY] = load_img("res/empty.bmp");
  imgs[MINE] = load_img("res/mine.bmp");
  imgs[REDMINE] = load_img("res/redmine.bmp");
  imgs[ERROR] = load_img("res/error.bmp");
  imgs[WIN] = load_img("res/win.bmp");
  imgs[FLAG] = load_img("res/flag.bmp");
  imgs[HINT] = load_img("res/hint.bmp");
  imgs[UNKNOW] = load_img("res/unknow.bmp");
  imgs[SAFE] = load_img("res/safe.bmp");
  imgs[DANGER] = load_img("res/danger.bmp");
  imgs[OFF] = load_img("res/off.bmp");
  imgs[MINUS] = load_img("res/minus.bmp");
  imgs[PLUS] = load_img("res/plus.bmp");
  imgs[X] = load_img("res/x.bmp");
  initialiser_fenetre(SCALE, SCALE, "Demineur");
  disable_auto_redraw();
  nb_x = 16;
  nb_y = 16;
  nb_mine = 40;
  nb_mine = 50;
  mode = 3;
}

int config() { //MODIF INPUT WINDOW
  resize_window(20 * SCALE, 7 * SCALE);
  while(true) {
    draw_rect(0, 19, 0, 6, OFF);
    draw_rect(1, 18, 1, 5, EMPTY);
    draw_cell(4, 3, X);
    draw_cell(8, 3, MINE);
    draw_cell(13, 4, MINE);
    draw_cell(14, 4, X);
    draw_cell(15, 4, 0);
    draw_cell(17, 2, WIN);
    draw_cell(17, 4, X);
    draw_number(2, 3, 2, nb_x);
    draw_number(5, 3, 2, nb_y);
    draw_number(9, 3, 3, nb_mine);
    draw_cell(2, 4, (nb_x + 10 > X_MAX) ? EMPTY : PLUS);
    draw_cell(2, 2, (nb_x - 10 < 9) ? EMPTY : MINUS);
    draw_cell(3, 4, (nb_x + 1 > X_MAX) ? EMPTY : PLUS);
    draw_cell(3, 2, (nb_x - 1 < 9) ? EMPTY : MINUS);
    draw_cell(5, 4, (nb_y + 10 > X_MAX) ? EMPTY : PLUS);
    draw_cell(5, 2, (nb_y - 10 < 9) ? EMPTY : MINUS);
    draw_cell(6, 4, (nb_y + 1 > X_MAX) ? EMPTY : PLUS);
    draw_cell(6, 2, (nb_y - 1 < 9) ? EMPTY : MINUS);
    draw_cell(9, 4, (nb_mine + 100 > (nb_x - 1) * (nb_y - 1)) ? EMPTY : PLUS);
    draw_cell(9, 2, (nb_mine - 100 < 10) ? EMPTY : MINUS);
    draw_cell(10, 4, (nb_mine + 10 > (nb_x - 1) * (nb_y - 1)) ? EMPTY : PLUS);
    draw_cell(10, 2, (nb_mine - 10 < 10) ? EMPTY : MINUS);
    draw_cell(11, 4, (nb_mine + 1 > (nb_x - 1) * (nb_y - 1)) ? EMPTY : PLUS);
    draw_cell(11, 2, (nb_mine - 1 < 10) ? EMPTY : MINUS);
    draw_cell(13, 3, (mode == 1) ? SAFE : DANGER);
    draw_cell(14, 3, (mode == 2) ? SAFE : DANGER);
    draw_cell(15, 3, (mode == 3) ? SAFE : DANGER);
    show_all();
    click = wait_mouse_click();
    convert_coord1();
    if(click.coord.x == 17 && click.coord.y == 2) {
      return 0;
    }
    if(click.coord.x == 17 && click.coord.y == 4) {
      return 1;
    }
    if(click.coord.x == 13 && click.coord.y == 3) {
      mode = 1;
    }
    if(click.coord.x == 14 && click.coord.y == 3) {
      mode = 2;
    }
    if(click.coord.x == 15 && click.coord.y == 3) {
      mode = 3;
    }
    if(click.coord.x == 2 && click.coord.y == 4 && nb_x + 10 <= X_MAX) {
      nb_x = nb_x + 10;
    }
    if(click.coord.x == 2 && click.coord.y == 2 && nb_x - 10 >= 9) {
      nb_x = nb_x - 10;
    }
    if(click.coord.x == 3 && click.coord.y == 4 && nb_x + 1 <= X_MAX) {
      nb_x = nb_x + 1;
    }
    if(click.coord.x == 3 && click.coord.y == 2 && nb_x - 1 >= 9) {
      nb_x = nb_x - 1;
    }
    if(click.coord.x == 5 && click.coord.y == 4 && nb_y + 10 <= Y_MAX) {
      nb_y = nb_y + 10;
    }
    if(click.coord.x == 5 && click.coord.y == 2 && nb_y - 10 >= 9) {
      nb_y = nb_y - 10;
    }
    if(click.coord.x == 6 && click.coord.y == 4 && nb_y + 1 <= Y_MAX) {
      nb_y = nb_y + 1;
    }
    if(click.coord.x == 6 && click.coord.y == 2 && nb_y - 1 >= 9) {
      nb_y = nb_y - 1;
    }
    if(click.coord.x == 9 && click.coord.y == 4 && nb_mine + 100 <= (nb_x - 1) * (nb_y - 1)) {
      nb_mine = nb_mine + 100;
    }
    if(click.coord.x == 9 && click.coord.y == 2 && nb_mine - 100 >= 10) {
      nb_mine = nb_mine - 100;
    }
    if(click.coord.x == 10 && click.coord.y == 4 && nb_mine + 10 <= (nb_x - 1) * (nb_y - 1)) {
      nb_mine = nb_mine + 10;
    }
    if(click.coord.x == 10 && click.coord.y == 2 && nb_mine - 10 >= 10) {
      nb_mine = nb_mine - 10;
    }
    if(click.coord.x == 11 && click.coord.y == 4 && nb_mine + 1 <= (nb_x - 1) * (nb_y - 1)) {
      nb_mine = nb_mine + 1;
    }
    if(click.coord.x == 11 && click.coord.y == 2 && nb_mine - 1 >= 10) {
      nb_mine = nb_mine - 1;
    }
    if(nb_mine > (nb_x - 1) * (nb_y - 1)) {
      nb_mine = (nb_x - 1) * (nb_y - 1);
    }
  }
}

void convert_coord1() {
  click.coord.x = click.coord.x / SCALE;
  click.coord.y = click.coord.y / SCALE;
}

/* Inutile
void convert_coord2() {
  click.coord.x = click.coord.x * SCALE;
  click.coord.y = click.coord.y * SCALE;
}
/**/

void border() {
  draw_rect(0, nb_x + 1, 0, nb_y + 1, OFF);
  draw_rect(1, nb_x, 1, nb_y, UNKNOW);
  draw_rect(nb_x + 2, nb_x + 6, 0, nb_y + 1, EMPTY);
  draw_cell(nb_x + 3, nb_y, FLAG);
  draw_cell(nb_x + 5, nb_y, X);
  draw_number(nb_x + 3, nb_y - 2, 3, nb_mine);
}

CELLULE init_cell() {
  CELLULE cell;
  cell.mine = 0;
  cell.number = 0;
  cell.display = 0;
  return cell;
}

void init_grid() {
  int i, j;
  for(i = 0; i < nb_x; i++)
    for(j = 0; j < nb_y; j++) {
      grid.cell[i][j] = init_cell();
    }
}

void generat_grid(int x, int y) {
  POINT mine;
  int i = 0, i1, j1;
  while(i < nb_mine) {
    mine.x = random_int(nb_x);
    mine.y = random_int(nb_y);
    if(has_mine(mine.x, mine.y)) {}
    else if(mine.x == x && mine.y == y && mode >= 2) {}
    else if(mine.x >= x - 1 && mine.x <= x + 1 && mine.y >= y - 1 && mine.y <= y + 1 && mode >= 3) {} //BLURP
    else {
      grid.cell[mine.x][mine.y].mine = 1;
      for(i1 = mine.x - 1; i1 <= mine.x + 1; i1++)
        for(j1 = mine.y - 1; j1 <= mine.y + 1; j1++)
          if(in_bbox(0, nb_x, 0, nb_y, i1, j1)) {
            grid.cell[i1][j1].number++;
          }
      i++;
    }
  }
}

void redraw_grid() { //BLURP
  POINT p;
  int i, j;
  for(j = 0; j < nb_y; j++)
    for(i = 0; i < nb_x; i++) {
      p.x = (i + 1);
      p.y = (j + 1);
      if(is_display(i, j, 0)) {
        draw_cell(p.x, p.y, UNKNOW);
      } else if(is_display(i, j, 4)) {
        draw_cell(p.x, p.y, FLAG);
      } else if(is_display(i, j, 5)) {
        draw_cell(p.x, p.y, HINT);
      } else if(is_display(i, j, 6)) {
        draw_cell(p.x, p.y, REDMINE);
      } else if(is_display(i, j, 7)) {
        draw_cell(p.x, p.y, WIN);
      } else if(is_display(i, j, 8)) {
        draw_cell(p.x, p.y, MINE);
      } else if(is_display(i, j, 9)) {
        draw_cell(p.x, p.y, ERROR);
      } else if(grid.cell[i][j].number == 0) {
        draw_cell(p.x, p.y, EMPTY);
      } else {
        draw_cell(p.x, p.y, grid.cell[i][j].number);
      }
    }
}

/*
//fonction générant trop d'iterations
void update_grid(int i, int j) {
  if(is_display(i, j, 0)) {
    set_display(i, j, 1);

    if(grid.cell[i][j].number == 0 && grid.cell[i][j].mine == 0) {
      int i1, j1;

      for(i1 = i - 1; i1 < i + 2; i1++)
        for(j1 = j - 1; j1 < j + 2; j1++)
          if(in_bbox(0, nb_x, 0, nb_y, i1, j1))
            update_grid(i1, j1);
    }

    redraw_grid();
    show_all();
    attendre(15);
  }
}
/**/

// 0=caché 1=affiché 2=à calculer 3=à reporter au prochain cycle
void update_grid(int i, int j) {
  if(!is_display(i, j, 4) && is_hide(i, j)) {
    set_display(i, j, 2);
    if(has_mine(i, j)) {
      end = 2;
      set_display(i, j, 6);
    }
    int update = 0;
    do {
      update = 0;
      for(j = 0; j < nb_y; j++)
        for(i = 0; i < nb_x; i++)
          if(is_display(i, j, 3)) {
            set_display(i, j, 2);
          }
      for(j = 0; j < nb_y; j++)
        for(i = 0; i < nb_x; i++)
          if(is_display(i, j, 2)) {
            set_display(i, j, 1);
            nb_hidden--;
            update = 1;
            if(grid.cell[i][j].number == 0) {
              int i1, j1;
              for(i1 = i - 1; i1 <= i + 1; i1++)
                for(j1 = j - 1; j1 <= j + 1; j1++)
                  if(in_bbox(0, nb_x, 0, nb_y, i1, j1) && is_hide(i1, j1)) {
                    set_display(i1, j1, 3);
                  }
            }
          }
      redraw_grid();
      show_all();
      attendre(15);
    } while(update == 1);
  }
}

void update_flag(int i, int j) {
  if(nb_mine - nb_flag > 0 && (is_display(i, j, 0) || is_display(i, j, 10) || is_display(i, j, 11))) {
    nb_flag++;
    if(has_mine(i, j)) {
      nb_flag_mine++;
    }
    set_display(i, j, 4);
  } else if(is_display(i, j, 4)) {
    nb_flag--;
    if(has_mine(i, j)) {
      nb_flag_mine--;
    }
    set_display(i, j, 5);
  } else if(is_display(i, j, 0) || is_display(i, j, 10) || is_display(i, j, 11)) {
    set_display(i, j, 5);
  } else if(is_display(i, j, 5)) {
    set_display(i, j, 0);
  }
  draw_number(nb_x + 3, nb_y - 2, 3, nb_mine - nb_flag);
  redraw_grid();
}

int win() {
  int i, j;
  for(j = 0; j < nb_y; j++)
    for(i = 0; i < nb_x; i++)
      if(has_mine(i, j)) {
        set_display(i, j, 7);
      }
  draw_cell(nb_x + 3, nb_y, WIN);
  redraw_grid();
  show_all();
  while(1) {
    click = wait_mouse_click();
    convert_coord1();
    if(click.coord.x == nb_x + 3 && click.coord.y == nb_y) {
      return 0;
    }
    if(click.coord.x == nb_x + 5 && click.coord.y == nb_y) {
      return 1;
    }
  }
}

int lose() {
  int i, j;
  for(j = 0; j < nb_y; j++)
    for(i = 0; i < nb_x; i++) {
      if(is_display(i, j, 4) && !has_mine(i, j)) {
        set_display(i, j, 9);
      } else if(!is_display(i, j, 4) && has_mine(i, j) && !is_display(i, j, 6)) {
        set_display(i, j, 8);
      } else if(is_display(i, j, 4) && has_mine(i, j)) {
        set_display(i, j, 7);
      }
    }
  draw_cell(nb_x + 3, nb_y, REDMINE);
  redraw_grid();
  show_all();
  while(1) {
    click = wait_mouse_click();
    convert_coord1();
    if(click.coord.x == nb_x + 3 && click.coord.y == nb_y) {
      return 0;
    }
    if(click.coord.x == nb_x + 5 && click.coord.y == nb_y) {
      return 1;
    }
  }
}

void reset_solver() {
  int i, j;
  for(i = 0; i < nb_x; i++)
    for(j = 0; j < nb_y; j++)
      if(is_display(i, j, 10) || is_display(i, j, 11)) {
        set_display(i, j, 0);
      }
}

void solver(int x, int y) {
  redraw_grid();
  int i, j, i1, j1, hide, update;
  do {
    update = 0;
    for(i = 0; i < nb_x; i++)
      for(j = 0; j < nb_y; j++) {
        hide = 0;
        if(is_display(i, j, 1)) {
          for(i1 = i - 1; i1 < i + 2; i1++)
            for(j1 = j - 1; j1 < j + 2; j1++)
              if(in_bbox(0, nb_x, 0, nb_y, i1, j1) && is_hide(i1, j1) && !is_display(i1, j1, 10)) {
                hide++;
              }
          if(hide == grid.cell[i][j].number)
            for(i1 = i - 1; i1 < i + 2; i1++)
              for(j1 = j - 1; j1 < j + 2; j1++)
                if(in_bbox(0, nb_x, 0, nb_y, i1, j1) && is_display(i1, j1, 0)) {
                  set_display(i1, j1, 11);
                  update = 1;
                }
        }
      }
    for(i = 0; i < nb_x; i++)
      for(j = 0; j < nb_y; j++) {
        hide = 0;
        if(is_display(i, j, 1)) {
          for(i1 = i - 1; i1 < i + 2; i1++)
            for(j1 = j - 1; j1 < j + 2; j1++)
              if(in_bbox(0, nb_x, 0, nb_y, i1, j1) && (is_display(i1, j1, 11) || is_display(i1, j1, 4))) {
                hide++;
              }
          if(hide == grid.cell[i][j].number)
            for(i1 = i - 1; i1 < i + 2; i1++)
              for(j1 = j - 1; j1 < j + 2; j1++)
                if(in_bbox(0, nb_x, 0, nb_y, i1, j1) && is_display(i1, j1, 0)) {
                  set_display(i1, j1, 10);
                  update = 1;
                }
        }
      }
  } while(update == 1);
  POINT p;
  for(j = 0; j < nb_y; j++)
    for(i = 0; i < nb_x; i++)
      if(in_bbox(x - 3, x + 3, y - 3, y + 3, i, j)) {
        p.x = (i + 1);
        p.y = (j + 1);
        if(is_display(i, j, 10)) {
          draw_cell(p.x, p.y, SAFE);
        } else if(is_display(i, j, 11)) {
          draw_cell(p.x, p.y, DANGER);
        }
      }
  reset_solver();
  show_all();
}

int game() {
  nb_flag = 0;
  nb_flag_mine = 0;
  nb_hidden = nb_x * nb_y;
  init_grid();
  POINT fenetre;
  fenetre.x = ((nb_x + 7) * SCALE);
  fenetre.y = ((nb_y + 2) * SCALE);
  resize_window(fenetre.x, fenetre.y);
  border();
  redraw_grid();
  int first_left_click = 0;
  end = 0;
  while(end == 0) {
    show_all();
    click = wait_mouse_click();
    convert_coord1();
    if(in_bbox(1, nb_x + 1, 1, nb_y + 1, click.coord.x, click.coord.y)) {
      if(first_left_click == 0 && click.button == 0) {
        generat_grid(click.coord.x - 1, click.coord.y - 1);
        first_left_click++;
      }
      if(click.button == 0) {
        update_grid(click.coord.x - 1, click.coord.y - 1);
      } else if(click.button == 1) {
        solver(click.coord.x - 1, click.coord.y - 1);
      } else if(click.button == 2) {
        update_flag(click.coord.x - 1, click.coord.y - 1);
      }
    } else {
      if(click.coord.x == nb_x + 3 && click.coord.y == nb_y) {
        return 0;
      }
      if(click.coord.x == nb_x + 5 && click.coord.y == nb_y) {
        return 1;
      }
    }
    if((nb_flag == nb_flag_mine && nb_flag_mine == nb_mine) || (nb_hidden == nb_mine)) {
      end = 1;
    }
  }
  if(end == 1) {
    return win();
  } else if(end == 2) {
    return lose();
  }
}

int main(int argc,  char** argv) {
  init();
  int quit = 0;
  while(quit == 0) {
    quit = config();
    if(quit == 0) {
      quit = game();
    }
  }
  exit(0);
}

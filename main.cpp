//
//  main.cpp
//  Self Pong
//
//  Created by Arturo Vergara on 9/18/11.
//  Copyright 2011 NoCode. All rights reserved.
//

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#define SCREEN_W 640
#define SCREEN_H 480
#define BALL_SIZE 12
#define PLAYER_SIZE_X 8
#define PLAYER_SIZE_Y 96
#define FPS 60
enum PLAYER_KEYS {
    KEY_W, KEY_S, KEY_UP, KEY_DOWN
};

int main(int argv, char *argc[]) {
    const char *font_file = "Arial Bold.ttf";
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *ball = NULL;
    ALLEGRO_BITMAP *player1 = NULL;
    ALLEGRO_BITMAP *player2 = NULL;
    ALLEGRO_FONT *font = NULL;
    float player1_x = 64;
    float player1_y = SCREEN_H / 2.0 - PLAYER_SIZE_Y / 2.0;
    float player2_x = SCREEN_W - PLAYER_SIZE_X - 64;
    float player2_y = SCREEN_H / 2.0 - PLAYER_SIZE_Y / 2.0;
    float ball_x = SCREEN_W / 2.0 - BALL_SIZE / 2.0;
    float ball_y = SCREEN_H /2.0 - BALL_SIZE / 2.0;
    float ball_dx = 6.0, ball_dy = 6.0;
    bool doexit = false;
    bool redraw = true;
    bool key[4] = { false, false, false, false };
    int scoreP1 = 0;
    int scoreP2 = 0;
    bool bounce = false;
    bool reset = false;
    bool wait = false;
    bool pointP1 = false;
    bool pointP2 = false;
    bool gameOver = false;
    bool waitForGameOver = false;
    
    if (!al_init()) {
        fprintf(stderr, "failed to init\n");
        return -1;
    }
    
    al_init_font_addon();
    al_init_ttf_addon();
    
    if (!al_install_keyboard()) {
        fprintf(stderr, "no keyboard\n");
        return -1;
    }
    
    timer = al_create_timer(1.0 / FPS);
    if (!timer) {
        fprintf(stderr, "no timer\n");
        return -1;
    }
    
    display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        fprintf(stderr, "no display\n");
        return -1;
    }
    
    ball = al_create_bitmap(BALL_SIZE, BALL_SIZE);
    if (!ball) {
        fprintf(stderr, "no ball\n");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    player1 = al_create_bitmap(PLAYER_SIZE_X, PLAYER_SIZE_Y);
    if (!player1) {
        fprintf(stderr, "no player\n");
        al_destroy_bitmap(ball);
        al_destroy_display(display);
        al_destroy_timer(timer);
    }
    
    player2 = al_create_bitmap(PLAYER_SIZE_X, PLAYER_SIZE_Y);
    if (!player2) {
        fprintf(stderr, "no player 2\n");
        al_destroy_bitmap(player1);
        al_destroy_bitmap(ball);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    font = al_load_font(font_file, 12, 0);
    if (!font) {
        fprintf(stderr, "No font\n");
        al_destroy_bitmap(player2);
        al_destroy_bitmap(player1);
        al_destroy_bitmap(ball);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    al_set_target_bitmap(ball);
    
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    al_set_target_bitmap(player1);
    
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    al_set_target_bitmap(player2);
    
    al_clear_to_color(al_map_rgb(255, 255, 255));
    
    al_set_target_bitmap(al_get_backbuffer(display));
    
    event_queue = al_create_event_queue();
    if (!event_queue) {
        fprintf(stderr, "no queue\n");
        al_destroy_font(font);
        al_destroy_bitmap(player2);
        al_destroy_bitmap(player1);
        al_destroy_bitmap(ball);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    
    al_register_event_source(event_queue, al_get_display_event_source(display));
    
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    al_clear_to_color(al_map_rgb(0, 0, 0));
    
    al_flip_display();
    
    al_start_timer(timer);
    
    while (!doexit) {
        
        if (wait) {
            wait = false;
            al_stop_timer(timer);
            al_rest(1.5);
            al_start_timer(timer);
        }
        
        if (waitForGameOver) {
            waitForGameOver = false;
            al_show_native_message_box(display, "Game Over", NULL, "GAME OVER", NULL, 0);
            al_rest(1.0);
            break;
        }
        
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        
        if (event.type == ALLEGRO_EVENT_TIMER) {
            
            if (scoreP2 == 10 || scoreP1 == 10) {
                gameOver = true;
            }
            
            if (ball_y <= 0 || ball_y >= SCREEN_H - BALL_SIZE) {
                ball_dy = -ball_dy;
            }
            
            if (((ball_y >= player1_y && ball_y <= player1_y + PLAYER_SIZE_Y) && (ball_x >= player1_x && ball_x <= player1_x + PLAYER_SIZE_X)) || ((ball_y >= player2_y && ball_y <= player2_y + PLAYER_SIZE_Y) && ((ball_x + BALL_SIZE) >= player2_x && (ball_x + BALL_SIZE) <= player2_x + PLAYER_SIZE_X))) {
                bounce = true;
            } else {
                bounce = false;
            }
            
            if (bounce) {
                ball_dx = -ball_dx;
            }
            
            ball_x += ball_dx;
            ball_y += ball_dy;
            
            if (ball_x <= 0 - BALL_SIZE) {
                scoreP2++;
                if (!gameOver) {
                    pointP2 = true;
                    reset = true;
                }
            }
            
            if (ball_x >= SCREEN_W) {
                scoreP1++;
                if (!gameOver) {
                    pointP1 = true;
                    reset = true;
                }
            }
            
            if (key[KEY_W] && player1_y >= 4.0) {
                player1_y -= 6.0;
            }
            
            if (key[KEY_S] && player1_y <= SCREEN_H - PLAYER_SIZE_Y - 4.0) {
                player1_y += 6.0;
            }
            
            if (key[KEY_UP] && player2_y >= 4.0) {
                player2_y -= 6.0;
            }
            
            if (key[KEY_DOWN] && player2_y <= SCREEN_H - PLAYER_SIZE_Y - 4.0) {
                player2_y += 6.0;
            }
                        
            redraw = true;
        } else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    key[KEY_W] = true;
                    break;
                case ALLEGRO_KEY_S:
                    key[KEY_S] = true;
                    break;
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = true;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = true;
                    break;
            }
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_W:
                    key[KEY_W] = false;
                    break;
                case ALLEGRO_KEY_S:
                    key[KEY_S] = false;
                    break;
                case ALLEGRO_KEY_UP:
                    key[KEY_UP] = false;
                    break;
                case ALLEGRO_KEY_DOWN:
                    key[KEY_DOWN] = false;
                    break;
            }
        }
        
        if (redraw && al_is_event_queue_empty(event_queue)) {
            
            if (gameOver) {
                redraw = false;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_flip_display();
                waitForGameOver = true;
            } else {
                redraw = false;
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_textf(font, al_map_rgb(255, 255, 255), 50, 50, 0, "Score %i", scoreP1);
                al_draw_textf(font, al_map_rgb(255, 255, 255), (SCREEN_W - 80), 50, 0, "Score %i", scoreP2);
                al_draw_bitmap(player1, player1_x, player1_y, 0);
                al_draw_bitmap(player2, player2_x, player2_y, 0);
                if (reset) {
                    reset = false;
                    ball_y = SCREEN_H / 2.0 - BALL_SIZE / 2.0;
                    ball_x = SCREEN_W / 2.0 - BALL_SIZE / 2.0;
                    player1_x = 64;
                    player1_y = SCREEN_H / 2.0 - PLAYER_SIZE_Y / 2.0;
                    player2_x = SCREEN_W - PLAYER_SIZE_X - 64;
                    player2_y = SCREEN_H / 2.0 - PLAYER_SIZE_Y / 2.0;
                    wait = true;
                    if (pointP1) {
                        pointP1 = false;
                        ball_dx = -6.0;
                        ball_dy = 6.0;
                    } else if (pointP2) {
                        pointP2 = false;
                        ball_dx = 6.0;
                        ball_dy = 6.0;
                    }
                }
                al_draw_bitmap(ball, ball_x, ball_y, 0);
                al_flip_display();
            } 
        }
    }
    
    al_destroy_font(font);
    al_destroy_bitmap(ball);
    al_destroy_bitmap(player1);
    al_destroy_bitmap(player2);
    al_destroy_display(display);    
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    
    return 0;
}

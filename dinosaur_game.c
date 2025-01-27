#include <ncurses.h>
#include <unistd.h>

int main() {
    int ground_y = 30;  // موقعیت زمین
    int dino_x = 10;    //موقعیت دایناسور
    int obstacle_x1 = 50;  // موقعیت موانع
    int obstacle_x2 = 125; 
    int cloud1_x = 25;  // موقعیت ابرها
    int cloud2_x = 45;
    int cloud3_x = 95;
    int score = 0;  // امتیاز
    float jump = 0;  // وضعیت پرش
    int ch;
    int delay = 100000;  // تاخیر بین فریم‌ها
    int background_color = 3;  // رنگ پس‌زمینه

    // تعریف دایناسور
    const char dino[6][20] = {
        "              __  ",
        "             / _)",
        "    _.----._/ /",
        "   /         /",
        " / (  | (  |",
        "/.-'|_|--|_|"
    };

    // تعریف مانع اول
    const char cactus1[10][20] = {
        "      w",
        "     / \\",
        "     | | ",
        "     | |  ",
        " /\\  | | /\\",
        " | (_| |_) |",
        " \\.'.'.'.  /",
        "   '--.'.--'",
        "     | |",
        "     | |",
    };
   //تعریف مانع دوم
    const char cactus2[10][20] = {
        "     ,*-.   ",
        "   _ |  |  _",
        "  | ||  | | | ",
        "  |   |_| | |",
        "  ----.     |",
        "      |  .--",
        "      |  |   ",
        "      |  |   ",
        "      |  |   ",
        "      |  |   ",
    };

    //تعریف ابراول
    const char cloud1[4][20] = {
        "   __   _",
        " _(  )_( )_",
        "(_  _     _)",
        " (_) (__)",
    };
   //تعریف ابر دوم
    const char cloud2[6][30] = {
        "                .-~~~-.",
        "        .- ~ ~-(       )_",
        "       /                  ~ -.",
        "       |                    /",
        "        \\                .' ",
        "           ~- . ____ . -~"
    };

    initscr();  // راه‌اندازی ncurses
    noecho();  // جلوگیری از نمایش کاراکترهای وارد شده
    curs_set(FALSE);  // مخفی کردن نشانگر ماوس
    timeout(0);  // تنظیم زمان‌بندی برای گرفتن ورودی
    start_color();  // شروع رنگ‌ها
    init_pair(1, COLOR_GREEN, COLOR_BLACK);  // تعریف رنگ‌ها
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    init_pair(4, COLOR_GREEN, COLOR_WHITE);
    init_pair(5, COLOR_BLUE, COLOR_WHITE);

    while (1) {
        // تغییر رنگ پس‌زمینه به طور متناوب
        if (score % 2 == 0) {
            background_color = 4;
        } else {
            background_color = 3;
        }
        bkgd(COLOR_PAIR(background_color));  // اعمال رنگ پس‌زمینه
        clear();  // پاک کردن صفحه

        // تنظیم رنگ‌ها بر اساس پس‌زمینه
        if (background_color == 4) {
            init_pair(1, COLOR_GREEN, COLOR_WHITE);
            init_pair(2, COLOR_BLACK, COLOR_WHITE);
            init_pair(5, COLOR_BLUE, COLOR_WHITE);
        } else {
            init_pair(1, COLOR_GREEN, COLOR_BLACK);
            init_pair(2, COLOR_WHITE, COLOR_BLACK);
            init_pair(5, COLOR_BLUE, COLOR_BLACK);
        }

        // نمایش ابرها
        attron(COLOR_PAIR(5));  
        for (int i = 0; i < 4; i++) {
            if (cloud1_x >= 0 && cloud1_x < COLS)
             mvprintw(i + 1, cloud1_x, "%s", cloud1[i]);
        }
        for (int i = 0; i < 4; i++) {
            if (cloud3_x >= 0 && cloud3_x < COLS)
             mvprintw(i + 3, cloud3_x, "%s", cloud1[i]);
        }
        for (int i = 0; i < 6; i++) {
            if (cloud2_x >= 0 && cloud2_x < COLS)
             mvprintw(i + 3, cloud2_x, "%s", cloud2[i]);
        }
        attroff(COLOR_PAIR(5));

        // نمایش زمین
        attron(COLOR_PAIR(1));  
        for (int i = 0; i < COLS; i++) {
            mvprintw(ground_y, i, "-");
        }
        attroff(COLOR_PAIR(1));

        attron(COLOR_PAIR(2));
        for (int i = 0; i < 6; i++) {
            mvprintw(ground_y - 6 + i - (int)jump, dino_x, "%s", dino[i]);
        }
        attroff(COLOR_PAIR(2));

        // نمایش موانع
        attron(COLOR_PAIR(1));
        for (int i = 0; i < 10; i++) {
            mvprintw(ground_y - 9 + i, obstacle_x1, "%s", cactus1[i]);
        }
        for (int i = 0; i < 10; i++) {
            mvprintw(ground_y - 9 + i, obstacle_x2, "%s", cactus2[i]);
        }
        attroff(COLOR_PAIR(1));
        mvprintw(1, 1, "Score: %d", score);  // نمایش امتیاز

        // بررسی برخورد با موانع
        int collision = 0;
        for (int i = 0; i < 5; i++) {
            if ((ground_y - 4 + i - jump >= ground_y - 9) &&
                ((obstacle_x1 >= dino_x && obstacle_x1 < dino_x + 15) ||
                 (obstacle_x2 >= dino_x && obstacle_x2 < dino_x + 15))) {
                collision = 1;
                break;
            }
        }
        if (collision) {
            mvprintw(15,70, "Game Over");  // نمایش پیغام پایان بازی
            refresh();
            usleep(2000000);  // تاخیر قبل از خروج
            break;
        }

        // حرکت موانع و ابرها
        obstacle_x1--;
        obstacle_x2--;
        if (obstacle_x1 < 0) obstacle_x1 = COLS - 1;
        if (obstacle_x2 < 0) obstacle_x2 = COLS - 1;
        if (obstacle_x1 == obstacle_x2) obstacle_x2 -= 10;
        cloud1_x--;
        cloud2_x--;
        cloud3_x--;
        if (cloud1_x < 0) cloud1_x = COLS - 1;
        if (cloud2_x < 0) cloud2_x = COLS - 1;
        if (cloud3_x < 0) cloud3_x = COLS - 1;

        // بررسی امتیاز و افزایش سرعت بازی
        if ((obstacle_x1 == dino_x) || (obstacle_x2 == dino_x)) {
            score++;
            if (score > 0) {
                delay -= 20000;
                if (delay <= 20000) delay = 10000;
            }
        }

        // گرفتن ورودی برای پرش
        ch = getch();
        if (ch == ' ') {
            jump=25;
        }
        if (jump > 0) {
            jump -= 0.5;
        }

        refresh();  // نمایش صفحه
        usleep(delay);  // تاخیر بین فریم‌ها
    }

    endwin();  // پایان استفاده از ncurses
    return 0;
}
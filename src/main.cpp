#include "libs.hpp"

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define CIRCLE_RADIUS 300

typedef struct s_vars {
    void *mlx;
    void *win;
    void *img;
    char *addr;
    int bits_per_pixel;
    int line_length;
    int endian;
} t_vars;

void my_mlx_pixel_put(t_vars *vars, int x, int y, int color)
{
    char *dst;

    dst = vars->addr + (y * vars->line_length + x * (vars->bits_per_pixel / 8));
    *(unsigned int*)dst = color;
}

int calculate_color(const Vector3& point, const Vector3& light_dir)
{
    float intensity = std::max(0.0f, point.normalize() * light_dir);
    intensity = pow(intensity, M_PI);
    int r = static_cast<int>(255 * intensity);
    int g = static_cast<int>(200 * intensity);
    int b = static_cast<int>(150 * intensity);
    return (r << 16) | (g << 8) | b;
}

void draw_circle(t_vars *vars, int center_x, int center_y, int radius)
{
    Vector3 light_dir = Vector3(1, 1, 1).normalize();
    
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            if (x*x + y*y <= radius*radius)
            {
                float z = sqrt(radius*radius - x*x - y*y);
                Vector3 point(x, y, z);
                
                int color = calculate_color(point, light_dir);
                
                int draw_x = center_x + x;
                int draw_y = center_y + y;
                if (draw_x >= 0 && draw_x < WINDOW_WIDTH && draw_y >= 0 && draw_y < WINDOW_HEIGHT)
                {
                    my_mlx_pixel_put(vars, draw_x, draw_y, color);
                }
            }
        }
    }
}

int key_hook(int keycode, t_vars *vars)
{
    std::cout << "Key pressed: " << keycode << std::endl;
    if (keycode == 65307) // ESC key
    {
        mlx_destroy_window(vars->mlx, vars->win);
        mlx_destroy_display(vars->mlx);
        std::exit(0);
    }
    return (0);
}

int main(void)
{
    t_vars vars;

    vars.mlx = mlx_init();
    if (!vars.mlx)
        return (1);

    vars.win = mlx_new_window(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT, (char *)"3D Circle Display");
    if (!vars.win)
    {
        mlx_destroy_display(vars.mlx);
        return (1);
    }

    vars.img = mlx_new_image(vars.mlx, WINDOW_WIDTH, WINDOW_HEIGHT);
    vars.addr = mlx_get_data_addr(vars.img, &vars.bits_per_pixel, &vars.line_length, &vars.endian);

    draw_circle(&vars, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, CIRCLE_RADIUS);

    mlx_put_image_to_window(vars.mlx, vars.win, vars.img, 0, 0);

    mlx_key_hook(vars.win, reinterpret_cast<int (*)(int, void *)>(key_hook), &vars);

    mlx_loop(vars.mlx);

    return (0);
}
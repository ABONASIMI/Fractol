# fractol Runtime Line-by-Line (شروع اجرا تا پایان)

این نسخه دقیقاً همان چیزی است که خواستی:
- نه فقط توضیح کلی
- نه فقط توضیح فایل‌به‌فایل
- بلکه «وقتی برنامه اجرا می‌شود، از کدام خط شروع می‌کند و هر خط چه می‌کند»

کدهای مرجع:
- `main.c`
- `utils.c`
- `events.c`
- `render.c`
- `fractol.h`

---

## 0) اجرای برنامه از کجا شروع می‌شود؟

وقتی می‌زنی:

```bash
./fractol julia -0.8 0.156
```

CPU وارد `main` می‌شود:
- `main.c:79`

---

## 1) `main` — مسیر اجرای واقعی خط‌به‌خط

کد:

```c
int	main(int ac, char **av)
{
	t_data	data;

	init_data(&data);
	if (!parse_args(ac, av, &data))
		return (1);
	if (setup_graphics(&data))
		return (1);
	render(&data);
	mlx_key_hook(data.win, key_hook, &data);
	mlx_mouse_hook(data.win, mouse_hook, &data);
	mlx_hook(data.win, 17, 0, close_hook, &data);
	mlx_loop(data.mlx);
	return (0);
}
```

### `main.c:81` → `t_data data;`
- چه می‌کند: یک object از نوع `t_data` روی stack می‌سازد.
- چرا لازم است: کل state برنامه را نگه می‌دارد (window, image, zoom, set...).
- اگر نبود: نمی‌توانستی state یکپارچه داشته باشی.

### `main.c:83` → `init_data(&data);`
- چه می‌کند: مقدار اولیه امن برای همه فیلدها می‌گذارد.
- بعدش برنامه می‌رود داخل `init_data` در `main.c:29`.

---

## 2) `init_data` — خط‌به‌خط (همین لحظه اجرا می‌شود)

کد:

```c
void	init_data(t_data *data)
{
	data->mlx = NULL;
	data->win = NULL;
	data->img = NULL;
	data->addr = NULL;
	data->bits_per_pixel = 0;
	data->line_length = 0;
	data->endian = 0;
	data->zoom = 1.0;
	data->shift_x = 0.0;
	data->shift_y = 0.0;
	data->julia_x = -0.744;
	data->julia_y = 0.148;
	data->color_shift = 0;
	data->set = MANDELBROT;
}
```

### `main.c:31` → `data->mlx = NULL;`
- چه می‌کند: pointer context را خالی می‌گذارد.
- چرا لازم است: قبل از `mlx_init` معتبر نیست.

### `main.c:32` → `data->win = NULL;`
- pointer پنجره هنوز ساخته نشده.

### `main.c:33` → `data->img = NULL;`
- pointer تصویر هنوز ساخته نشده.

### `main.c:34` → `data->addr = NULL;`
- آدرس buffer هنوز معلوم نیست.

### `main.c:35` → `bits_per_pixel = 0;`
- metadata اولیه امن.

### `main.c:36` → `line_length = 0;`
- metadata اولیه امن.

### `main.c:37` → `endian = 0;`
- metadata اولیه امن.

### `main.c:38` → `zoom = 1.0;`
- zoom پایه (بدون بزرگنمایی).

### `main.c:39` → `shift_x = 0.0;`
- جابجایی افقی اولیه.

### `main.c:40` → `shift_y = 0.0;`
- جابجایی عمودی اولیه.

### `main.c:41` → `julia_x = -0.744;`
- `c` پیش‌فرض Julia (real).

### `main.c:42` → `julia_y = 0.148;`
- `c` پیش‌فرض Julia (imag).

### `main.c:43` → `color_shift = 0;`
- پالت رنگ از حالت پایه شروع می‌شود.

### `main.c:44` → `set = MANDELBROT;`
- set پیش‌فرض (اگر parse بعداً تغییر ندهد).

خروج از `init_data` و برگشت به `main`.

---

## 3) برگشت به `main` و parse ورودی

### `main.c:84` → `if (!parse_args(ac, av, &data))`
- چه می‌کند: ورودی command line را بررسی می‌کند.
- `!` یعنی اگر `parse_args` صفر برگرداند (fail)، وارد `if` شو.
- حالا برنامه می‌رود داخل `parse_args` در `utils.c:85`.

---

## 4) `parse_args` — خط‌به‌خط با مسیر واقعی همین اجرا

فرض اجرای واقعی ما:

```bash
./fractol julia -0.8 0.156
```

پس:
- `ac = 4`
- `av[1] = "julia"`
- `av[2] = "-0.8"`
- `av[3] = "0.156"`

کد:

```c
int	parse_args(int ac, char **av, t_data *data)
{
	if (ac == 2 && streq(av[1], "mandelbrot"))
		data->set = MANDELBROT;
	else if (ac == 2 && (streq(av[1], "burningship")
			|| streq(av[1], "burning_ship")))
		data->set = BURNING_SHIP;
	else if (ac == 2 && streq(av[1], "julia"))
		data->set = JULIA;
	else if (ac == 4 && streq(av[1], "julia")
		&& ft_atof_strict(av[2], &data->julia_x)
		&& ft_atof_strict(av[3], &data->julia_y))
		data->set = JULIA;
	else
		return (usage());
	set_default_view(data);
	return (1);
}
```

### `utils.c:87` → `if (ac == 2 && streq(..."mandelbrot"))`
- چون `ac=4`، بخش `ac == 2` **false** می‌شود.
- به خاطر short-circuit، `streq` اصلاً صدا زده نمی‌شود.

### `utils.c:89` → `else if (ac == 2 && (...burningship...))`
- باز هم `ac == 2` false.
- هیچ `streq` اجرا نمی‌شود.

### `utils.c:92` → `else if (ac == 2 && streq(..."julia"))`
- باز false.

### `utils.c:94` → `else if (ac == 4 && streq(av[1], "julia") ... )`
- `ac == 4` true.
- حالا `streq(av[1], "julia")` اجرا می‌شود.

#### `streq` اجرا می‌شود (`utils.c:15`)

کد:

```c
static int	streq(char *a, char *b)
{
	while (*a && *b && *a == *b)
	{
		++a;
		++b;
	}
	return (*a == '\0' && *b == '\0');
}
```

- کاراکتر به کاراکتر `"julia"` با `"julia"` مقایسه می‌شود.
- در انتها هر دو به `\0` می‌رسند → `return 1`.

برگشت به شرط `utils.c:94`.

### `utils.c:95` → `ft_atof_strict(av[2], &data->julia_x)`
- `av[2] = "-0.8"`.
- اگر parse موفق شود، مقدار در `data->julia_x` ریخته می‌شود.

#### اجرای `ft_atof_strict` (`utils.c:56`) برای `"-0.8"`

1. `if (!s || !*s || !out)` → false (ورودی معتبر است).
2. `res = 0.0; frac = 0.1; sign = 1; started = 0;`
3. `if (*s == '-' || *s == '+')` true چون `'-'`.
4. `sign = -2 * (*s++ == '-') + 1;` → `sign = -1`, و `s` یک کاراکتر جلو می‌رود.
5. `if (*s >= '0' && *s <= '9') started = 1;` برای `'0'` true.
6. `parse_digits(..., frac_mode=0)` بخش صحیح را parse می‌کند (`0`).
7. `if (*s == '.') ++s;` از ممیز رد می‌شود.
8. `if (*s >= '0' && *s <= '9') started = 1;` برای `'8'` true.
9. `parse_digits(..., frac_mode=1)` بخش اعشاری را parse می‌کند (`0.8`).
10. `if (*s || !started)` false (چیزی اضافه نمانده).
11. `*out = res * sign` → `-0.8`.
12. `return 1`.

### `utils.c:96` → `ft_atof_strict(av[3], &data->julia_y)`
- `av[3] = "0.156"` parse می‌شود.
- خروجی: `data->julia_y = 0.156`.

### `utils.c:97` → `data->set = JULIA;`
- set انتخابی نهایی: Julia.

### `utils.c:100` → `set_default_view(data);`
- بعد از parse، view مناسب set تنظیم می‌شود.
- حالا برنامه می‌رود `events.c:22`.

---

## 5) `set_default_view` — خط‌به‌خط در مسیر فعلی

کد:

```c
void	set_default_view(t_data *data)
{
	data->zoom = 1.0;
	data->color_shift = 0;
	if (data->set == JULIA)
	{
		data->shift_x = 0.0;
		data->shift_y = 0.0;
		return ;
	}
	data->shift_x = -0.5;
	data->shift_y = 0.0;
	if (data->set == BURNING_SHIP)
		data->shift_y = -0.5;
}
```

### اجرای واقعی برای set = JULIA
1. `zoom = 1.0`
2. `color_shift = 0`
3. شرط `data->set == JULIA` true.
4. `shift_x = 0.0`
5. `shift_y = 0.0`
6. `return` → خطوط Mandelbrot/BurningShip اجرا نمی‌شوند.

برگشت به `parse_args`.

### `utils.c:101` → `return (1);`
- parse موفق.
- برگشت به `main`.

---

## 6) برگشت به `main` و setup گرافیک

### `main.c:84` شرط `if (!parse_args...)`
- چون parse `1` برگرداند، `!1 = 0`، پس وارد `return (1)` نمی‌شود.

### `main.c:86` → `if (setup_graphics(&data))`
- می‌رود داخل `setup_graphics` (`main.c:60`).

---

## 7) `setup_graphics` — خط‌به‌خط

کد:

```c
int	setup_graphics(t_data *data)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		return (put_err("Error: mlx_init failed\n"));
	data->win = mlx_new_window(data->mlx, WIDTH, HEIGHT, "fract-ol");
	if (!data->win)
		return (destroy_data(data), put_err("Error: mlx_new_window failed\n"));
	data->img = mlx_new_image(data->mlx, WIDTH, HEIGHT);
	if (!data->img)
		return (destroy_data(data), put_err("Error: mlx_new_image failed\n"));
	data->addr = mlx_get_data_addr(data->img, &data->bits_per_pixel,
			&data->line_length, &data->endian);
	if (!data->addr)
		return (destroy_data(data),
			put_err("Error: mlx_get_data_addr failed\n"));
	return (0);
}
```

1. `mlx_init()` context می‌سازد.
2. اگر fail شود، error چاپ و خروج.
3. `mlx_new_window(..., WIDTH, HEIGHT, "fract-ol")` پنجره می‌سازد.
4. اگر fail شود، cleanup و خروج.
5. `mlx_new_image(...)` buffer تصویر می‌سازد.
6. اگر fail شود، cleanup و خروج.
7. `mlx_get_data_addr(...)` آدرس raw pixel memory و metadata می‌گیرد.
8. اگر fail شود، cleanup و خروج.
9. `return 0` یعنی setup موفق.

برگشت به `main`.

### `main.c:86` شرط
- چون setup `0` داده، وارد `return (1)` نمی‌شود.

---

## 8) اولین رندر (`main.c:88`)

### `main.c:88` → `render(&data);`
- برنامه وارد `render` در `render.c:94` می‌شود.

---

## 9) `render` — خط‌به‌خط (واقعا چه اتفاقی می‌افتد)

کد:

```c
void	render(t_data *data)
{
	t_render	r;
	int			*row;

	r.sx = 4.0 / (WIDTH * data->zoom);
	r.sy = 4.0 / (HEIGHT * data->zoom);
	r.start_x = (-WIDTH / 2.0) * r.sx + data->shift_x;
	r.start_y = (-HEIGHT / 2.0) * r.sy + data->shift_y;
	r.y = 0;
	while (r.y < HEIGHT)
	{
		row = (int *)(data->addr + r.y * data->line_length);
		r.x = 0;
		while (r.x < WIDTH)
		{
			r.cx = r.start_x + r.x * r.sx;
			r.cy = r.start_y + r.y * r.sy;
			row[r.x] = get_palette_color(get_point_iter(data, r.cx, r.cy),
					data->color_shift);
			++r.x;
		}
		++r.y;
	}
	mlx_put_image_to_window(data->mlx, data->win, data->img, 0, 0);
}
```

### خطوط setup mapping
1. `r.sx` اندازه هر pixel روی محور x در صفحه مختلط.
2. `r.sy` اندازه هر pixel روی محور y.
3. `r.start_x` مختصات مختلط پیکسل x=0.
4. `r.start_y` مختصات مختلط پیکسل y=0.
5. `r.y = 0` شروع از سطر اول.

### حلقه y (`while (r.y < HEIGHT)`)
- برای هر سطر تصویر یک بار اجرا می‌شود (800 بار).

### `row = (int *)(data->addr + r.y * data->line_length);`
- آدرس ابتدای سطر `r.y` در buffer.

### حلقه x (`while (r.x < WIDTH)`)
- برای هر پیکسل سطر اجرا می‌شود (800 بار).

### `r.cx = ...` و `r.cy = ...`
- پیکسل `(x,y)` به نقطه مختلط `(cx, cy)` تبدیل می‌شود.

### `get_point_iter(...)`
- تعداد iteration تا escape را برای همین نقطه می‌دهد.

### `get_palette_color(...)`
- iteration را به رنگ تبدیل می‌کند (با `color_shift`).

### `row[r.x] = ...`
- رنگ پیکسل داخل buffer نوشته می‌شود.

### پایان
- بعد از تمام pixelها، `mlx_put_image_to_window` تصویر را روی پنجره می‌گذارد.

---

## 10) داخل `get_point_iter` — ریاضی دقیق هر پیکسل

کد:

```c
int	get_point_iter(t_data *data, double cx, double cy)
{
	double	z[2];
	double	tmp;
	int		i;

	init_iter(data, &cx, &cy, z);
	i = 0;
	while (i < MAX_ITER && (z[0] * z[0] + z[1] * z[1]) < 4.0)
	{
		if (data->set == BURNING_SHIP)
		{
			z[0] = fabs(z[0]);
			z[1] = fabs(z[1]);
		}
		tmp = z[0] * z[0] - z[1] * z[1] + cx;
		z[1] = 2.0 * z[0] * z[1] + cy;
		z[0] = tmp;
		++i;
	}
	return (i);
}
```

### `init_iter(data, &cx, &cy, z)`
- اگر `JULIA`:
  - `z0 = pixel`
  - `c = (julia_x, julia_y)`
- اگر `MANDELBROT/BURNING_SHIP`:
  - `z0 = 0`
  - `c = pixel`

### حلقه while
- شرط 1: `i < MAX_ITER` (حداکثر 100 تکرار)
- شرط 2: `|z|^2 < 4.0` یعنی هنوز escape نکرده.

### branch Burning Ship
- قبل از squaring، مختصات `z` قدرمطلق می‌شود.

### update فرمول
- `tmp = x^2 - y^2 + cx`
- `y = 2xy + cy`
- `x = tmp`
- `i++`

### `return (i)`
- اگر نزدیک set باشد `i` بالا.
- اگر سریع فرار کند `i` پایین.

---

## 11) داخل `get_palette_color` و `get_color`

### `get_palette_color`
1. اولین بار `palette` ساخته می‌شود (`ready == 0`).
2. برای `0..MAX_ITER` از `get_color` رنگ می‌سازد.
3. اگر `iter == MAX_ITER` رنگ داخل set (`palette[MAX_ITER]` = سیاه).
4. در غیر این‌صورت با `shift` index جابه‌جا می‌شود.

### `get_color`
1. `iter == MAX_ITER` -> `0x000000`.
2. `t = iter / MAX_ITER`.
3. فرمول polynomial برای RGB.
4. برگرداندن رنگ packed.

---

## 12) برگشت به `main` بعد از اولین render

### `main.c:89` → `mlx_key_hook(data.win, key_hook, &data);`
- callback کیبورد ثبت می‌شود.

### `main.c:90` → `mlx_mouse_hook(data.win, mouse_hook, &data);`
- callback mouse ثبت می‌شود.

### `main.c:91` → `mlx_hook(data.win, 17, 0, close_hook, &data);`
- callback بستن پنجره (X) ثبت می‌شود.

### `main.c:92` → `mlx_loop(data.mlx);`
- برنامه وارد event loop می‌شود و منتظر event می‌ماند.
- از اینجا به بعد، flow با eventها جلو می‌رود.

---

## 13) مسیر Runtime هنگام Eventها

## 13.1) Mouse wheel (zoom)

وقتی wheel بالا/پایین می‌چرخد:
- وارد `mouse_hook` (`events.c:56`) می‌شود.

### `mouse_hook` خط‌به‌خط
1. اگر `(x,y)` بیرون پنجره باشد: `return 0`.
2. `button == 4` -> `zoom_at_point(..., ZOOM_FACTOR)`.
3. `button == 5` -> `zoom_at_point(..., 1.0 / ZOOM_FACTOR)`.
4. در غیر این‌صورت ignore.
5. `render(data)` دوباره صدا می‌شود.

### `zoom_at_point` خط‌به‌خط
1. scale فعلی را حساب می‌کند.
2. مختصات مختلط زیر mouse را قبل zoom ذخیره می‌کند.
3. `data->zoom *= factor`.
4. scale جدید را حساب می‌کند.
5. `shift_x/shift_y` را طوری تنظیم می‌کند که همان نقطه زیر mouse بماند.

---

## 13.2) Keyboard

وقتی کلید می‌زنی:
- وارد `key_hook` (`events.c:70`) می‌شود.

### `key_hook` خط‌به‌خط
1. `XK_Escape` -> `close_hook(data)`.
2. `XK_Left` -> `shift_x -= MOVE_STEP / zoom`.
3. `XK_Right` -> `shift_x += MOVE_STEP / zoom`.
4. `XK_Up` -> `shift_y -= MOVE_STEP / zoom`.
5. `XK_Down` -> `shift_y += MOVE_STEP / zoom`.
6. `XK_c/XK_C` -> `color_shift += 2`.
7. `XK_v/XK_V` -> `color_shift -= 2`.
8. `XK_r/XK_R` -> `set_default_view(data)`.
9. اگر key پشتیبانی نشود: `return 0`.
10. در مسیر معتبر: `render(data)` برای اعمال تغییر.

---

## 13.3) Close window / ESC

### `close_hook` (`events.c:15`)
1. `destroy_data(data)` cleanup منابع MLX.
2. `exit(0)` خروج.

### `destroy_data` (`main.c:47`)
1. اگر image و mlx معتبرند: `mlx_destroy_image`.
2. اگر window و mlx معتبرند: `mlx_destroy_window`.
3. اگر mlx معتبر است:
   - `mlx_destroy_display`
   - `free(data->mlx)`

---

## 14) مسیر خطاها (Runtime واقعی)

## 14.1) ورودی غلط
- `main.c:84` -> `parse_args` صفر برمی‌گرداند.
- `main` در `main.c:85` با `return 1` خارج می‌شود.
- قبلش `usage()` در `utils.c:25` متن راهنما را چاپ کرده.

## 14.2) خطای گرافیکی
- اگر `mlx_init`/`mlx_new_window`/`mlx_new_image`/`mlx_get_data_addr` fail شوند:
- `setup_graphics` مقدار `1` برمی‌گرداند.
- `main` در `main.c:87` خارج می‌شود.

---

## 15) خلاصه خیلی کوتاه Flow کامل

1. Start: `main.c:79`
2. Init state: `init_data`
3. Parse CLI: `parse_args`
4. Set default camera: `set_default_view`
5. Init MLX: `setup_graphics`
6. First draw: `render`
7. Register callbacks
8. Enter `mlx_loop`
9. هر event -> update state -> `render`
10. خروج با ESC یا close button -> `destroy_data` -> `exit(0)`

---

## 16) Trace کامل Runtime برای همه حالت‌های اجرا

در این بخش، دقیقاً مسیر اجرا برای همه ورودی‌های اصلی را می‌بینی.

### 16.1) حالت `mandelbrot`

فرمان:

```bash
./fractol mandelbrot
```

Flow دقیق:
1. `main.c:79` شروع.
2. `init_data` اجرا می‌شود.
3. `parse_args`:
   - شرط `ac == 2 && streq(av[1], "mandelbrot")` true.
   - `data->set = MANDELBROT`.
   - `set_default_view`:
     - `zoom = 1.0`
     - `color_shift = 0`
     - چون `JULIA` نیست:
       - `shift_x = -0.5`
       - `shift_y = 0.0`
4. `setup_graphics` موفق.
5. `render`:
   - در `init_iter` چون set = `MANDELBROT`:
     - `z0 = 0`
     - `c = pixel`
6. حلقه event و تعامل کاربر.

### 16.2) حالت `julia` بدون پارامتر

فرمان:

```bash
./fractol julia
```

Flow دقیق:
1. `main` و `init_data`.
2. `parse_args`:
   - شرط `ac == 2 && streq(..., "julia")` true.
   - `data->set = JULIA`.
   - `set_default_view`:
     - `zoom = 1.0`
     - `color_shift = 0`
     - `shift_x = 0.0`
     - `shift_y = 0.0`
3. مقادیر Julia همان defaultهای `init_data` می‌مانند:
   - `julia_x = -0.744`
   - `julia_y = 0.148`
4. در `render/get_point_iter`:
   - `z0 = pixel`
   - `c = (-0.744, 0.148)`

### 16.3) حالت `julia` با پارامتر

فرمان:

```bash
./fractol julia -0.8 0.156
```

Flow:
1. `parse_args` می‌رسد به شرط `ac == 4 && streq(..., "julia") && ...`.
2. `ft_atof_strict("-0.8", &julia_x)` -> success.
3. `ft_atof_strict("0.156", &julia_y)` -> success.
4. `set = JULIA`, بعد `set_default_view`.
5. حالا Julia با `c = (-0.8, 0.156)` رسم می‌شود.

### 16.4) حالت `burningship`

فرمان:

```bash
./fractol burningship
```

Flow:
1. `parse_args` شرط دوم را true می‌کند.
2. `set = BURNING_SHIP`.
3. `set_default_view`:
   - `shift_x = -0.5`
   - `shift_y = -0.5`
4. در `get_point_iter`:
   - قبل از update:
     - `z[0] = fabs(z[0])`
     - `z[1] = fabs(z[1])`

### 16.5) حالت ورودی غلط

فرمان:

```bash
./fractol abc
```

Flow:
1. هیچ شرطی در `parse_args` match نمی‌شود.
2. `return usage();` اجرا می‌شود (مقدار 0).
3. `main`: `if (!parse_args(...)) return (1);`
4. برنامه با کد خطا خارج می‌شود.

---

## 17) Line-by-Line تکمیلی برای helperهایی که کوتاه بودند

## 17.1) `put_err` در `main.c`

کد:

```c
static int	put_err(char *msg)
{
	int	len;
	int	w;

	len = 0;
	while (msg[len])
		++len;
	w = write(2, msg, len);
	if (w < 0)
		return (1);
	return (1);
}
```

توضیح:
1. `static` یعنی فقط داخل همین فایل دیده می‌شود.
2. `len` طول message.
3. `w` نتیجه `write`.
4. loop طول رشته را می‌شمارد.
5. `write(2,...)` چاپ روی `stderr`.
6. حتی اگر write fail شود، همچنان error-code (`1`) می‌دهد.

---

## 17.2) `usage` در `utils.c`

1. `static char msg[] = ...` پیام ثابت راهنما.
2. `write(1, msg, sizeof(msg)-1)` چاپ کل پیام.
3. برگشت `0` برای fail در parse.

---

## 17.3) `parse_digits` در `utils.c`

1. تا وقتی digit باشد loop ادامه می‌دهد.
2. اگر `frac_mode = 0`:
   - رقم جدید به بخش صحیح اضافه می‌شود.
3. اگر `frac_mode = 1`:
   - رقم جدید به بخش اعشاری اضافه می‌شود.
   - `frac` هر بار 10 برابر کوچک‌تر می‌شود (`0.1`, `0.01`, ...).
4. در هر گام `(*s)++` یعنی parser جلو می‌رود.

---

## 17.4) `init_iter` در `render.c`

1. اگر set = `JULIA`:
   - `z0 = pixel` (در `z[0], z[1]`)
   - `c = julia_x + i*julia_y` (با overwrite روی `cx,cy`)
2. در غیر این‌صورت:
   - `z0 = 0`
   - `c = pixel` (همان `cx,cy` اصلی)

این دقیقاً فرق ریاضی Julia با Mandelbrot/BurningShip است.

---

## 18) مثال عددی واقعی (Iteration-by-Iteration)

## 18.1) Mandelbrot برای نقطه `c = 0 + 0i`

شروع:
- `z0 = 0`

iteration:
1. `z1 = z0^2 + c = 0 + 0 = 0`
2. `z2 = 0`
3. ... همیشه 0

نتیجه:
- escape نمی‌کند
- `iter = MAX_ITER`
- رنگ: سیاه

## 18.2) Mandelbrot برای نقطه `c = 2 + 0i`

شروع:
- `z0 = 0`

iteration:
1. `z1 = 0^2 + 2 = 2`
2. `|z1|^2 = 4` → شرط while دیگر برقرار نیست (`< 4.0` باید باشد)

نتیجه:
- خیلی سریع escape
- iteration کم
- رنگ روشن/خارج از set

## 18.3) Julia نمونه با `c = -0.8 + 0.156i`

فرض یک پیکسل که map شده به:
- `z0 = 0.2 + 0.3i`

iteration 1:
- `x' = x^2 - y^2 + cx = 0.04 - 0.09 - 0.8 = -0.85`
- `y' = 2xy + cy = 2*0.2*0.3 + 0.156 = 0.276`
- `z1 = -0.85 + 0.276i`

iterationهای بعد همین‌طور ادامه پیدا می‌کند تا escape یا رسیدن به `MAX_ITER`.

---

## 19) Mapping پیکسل به صفحه مختلط (خیلی مهم)

در `render`:

```c
r.sx = 4.0 / (WIDTH * zoom);
r.sy = 4.0 / (HEIGHT * zoom);
r.start_x = (-WIDTH / 2.0) * r.sx + shift_x;
r.start_y = (-HEIGHT / 2.0) * r.sy + shift_y;
cx = r.start_x + x * r.sx;
cy = r.start_y + y * r.sy;
```

معنی:
1. بازه پایه تقریبی محور: طول 4 واحد.
2. وقتی `zoom` زیاد می‌شود، `sx/sy` کوچک‌تر می‌شوند (یعنی zoom in).
3. `shift_x/shift_y` پنجره دید را جابه‌جا می‌کنند.
4. هر پیکسل `(x,y)` یک نقطه `(cx,cy)` در صفحه مختلط می‌شود.

مثال:
- `WIDTH = 800`, `zoom = 1`
- `sx = 4/800 = 0.005`
- اگر `shift_x = -0.5`، پیکسل وسط (`x=400`) می‌شود:
  - `cx = start_x + 400*0.005 = -0.5`

---

## 20) چرا `|z|^2 < 4.0`؟

در ریاضی fractal (برای این خانواده):
- اگر `|z| > 2` شود، مسیر قطعاً به بی‌نهایت می‌رود.
- برای سرعت، به‌جای `sqrt(x^2+y^2) > 2` از
  `x^2 + y^2 > 4` استفاده می‌شود.

مزیت:
- بدون `sqrt`
- سریع‌تر در هر پیکسل.

---

## 21) جزئیات Zoom حول Mouse (با مثال)

هدف: نقطه زیر cursor بعد از zoom هم همان نقطه بماند.

فرآیند:
1. نقطه مختلط زیر mouse قبل از zoom را حساب کن (`mouse_re`, `mouse_im`).
2. zoom را تغییر بده.
3. scale جدید را حساب کن.
4. shift جدید را طوری تنظیم کن که:
   - همان pixel cursor دوباره به همان `mouse_re/mouse_im` map شود.

اگر این کار را نکنی:
- zoom همیشه حول مرکز انجام می‌شود.
- حس کنترل ضعیف‌تر می‌شود.

---

## 22) چرا حرکت با `MOVE_STEP / zoom`؟

اگر حرکت ثابت باشد:
- در zoom بالا، یک step خیلی زیاد دیده می‌شود.

با تقسیم بر zoom:
- هر چه zoom بیشتر، حرکت ظریف‌تر.
- کنترل بهتر در deep zoom.

---

## 23) حافظه و Resource Lifetime

## 23.1) چه چیزهایی allocate می‌شوند؟
1. `mlx_init` -> MLX context.
2. `mlx_new_window` -> window resource.
3. `mlx_new_image` -> image resource.

## 23.2) چه چیزهایی آزاد می‌شوند؟
در `destroy_data`:
1. image
2. window
3. display + free(mlx)

## 23.3) چه چیزهایی stack هستند؟
- `t_data data;` در `main` (stack).
- `t_render r;` در `render` (stack).

## 23.4) static lifetime
- `palette` و `ready` در `get_palette_color`.

---

## 24) پیچیدگی زمانی و کارایی

هر frame:
- حلقه روی `WIDTH * HEIGHT` پیکسل.
- برای هر پیکسل تا `MAX_ITER` iteration.

تقریب:
- `O(WIDTH * HEIGHT * MAX_ITER)`
- با مقادیر فعلی: `800 * 800 * 100 = 64,000,000` گام (حداکثر).

نکته performance:
- precompute palette باعث می‌شود رنگ‌دهی سریع‌تر شود.

---

## 25) چک‌لیست ارزیابی (Mandatory + Bonus)

Mandatory:
1. `./fractol mandelbrot` کار می‌کند.
2. `./fractol julia` کار می‌کند.
3. `./fractol julia <x> <y>` کار می‌کند.
4. wheel zoom کار می‌کند.
5. ESC خروج تمیز.
6. close button خروج تمیز.
7. usage برای ورودی غلط چاپ می‌شود.
8. رنگ‌بندی depth وجود دارد.

Bonus:
1. `burningship` فعال است.
2. zoom بر اساس mouse position.
3. حرکت با arrow keys.
4. color shift با `C`/`V`.

---

## 26) سوال‌های رایج evaluator + جواب کوتاه

1. چرا `MAX_ITER = 100`؟
- تعادل speed/quality. بیشترش جزئیات بیشتر می‌دهد ولی کندتر می‌شود.

2. چرا از `|z|^2 < 4` استفاده کردی؟
- معادل `|z| < 2` است ولی بدون `sqrt` و سریع‌تر.

3. فرق Mandelbrot و Julia در کد تو چیست؟
- در `init_iter`:  
  Mandelbrot/BurningShip: `z0=0, c=pixel`  
  Julia: `z0=pixel, c=constant`

4. رنگ‌ها چطور تولید می‌شوند؟
- `iter` نرمال می‌شود به `t` و با فرمول polynomial به RGB تبدیل می‌شود.

5. چرا `set_default_view` بعد از parse صدا زده می‌شود؟
- چون framing مناسب هر fractal متفاوت است و بعد از مشخص شدن set باید تنظیم شود.

---

## 27) اشکال‌زدایی سریع (اگر چیزی کار نکرد)

1. اگر برنامه باز نشد:
- X11/Display را چک کن.
- `mlx_init` ممکن است fail شود.

2. اگر Julia با پارامتر کار نکرد:
- ورودی عددی strict است. `12a` یا `.` reject می‌شود.

3. اگر zoom عجیب شد:
- `zoom` صفر یا منفی نمی‌شود (factorها کنترل‌شده‌اند).

4. اگر رنگ‌ها تغییر نکرد:
- کلیدهای `C` یا `V` بزن، `color_shift` تغییر می‌کند و render مجدد باید رخ دهد.

---

## 28) نقشه ذهنی نهایی (Mental Model)

`fractol` = یک loop گرافیکی event-driven:
1. State initialize.
2. Input parse.
3. Graphics setup.
4. Draw all pixels using complex math.
5. Wait for events.
6. On event: update state -> redraw.
7. On close: cleanup resources -> exit.

اگر این 7 قدم را بفهمی، کل پروژه را واقعاً فهمیده‌ای.

---

## 29) ریاضی پایه‌ای که باید 100٪ بلد باشی

برای این پروژه، هسته ریاضی این است:

- عدد مختلط: `z = x + i y`
- در کد:
  - `x` همان `z[0]`
  - `y` همان `z[1]`

### 29.1) مشتق مستقیم فرمول `z^2`

اگر:
- `z = x + i y`

آنگاه:
- `z^2 = (x + i y)^2`
- `z^2 = x^2 + 2ixy + i^2 y^2`
- چون `i^2 = -1`:
- `z^2 = (x^2 - y^2) + i(2xy)`

پس وقتی می‌گوییم:
- `z_{n+1} = z_n^2 + c`

یعنی:
- `x_{n+1} = x_n^2 - y_n^2 + c_x`
- `y_{n+1} = 2 x_n y_n + c_y`

این دقیقاً همان کد تو است در `render.c`:

```c
tmp = z[0] * z[0] - z[1] * z[1] + cx;
z[1] = 2.0 * z[0] * z[1] + cy;
z[0] = tmp;
```

### 29.2) چرا از `tmp` استفاده شده؟

اگر مستقیم بنویسی:
- اول `z[0]` را آپدیت کنی
- بعد برای `z[1]` از `z[0]` استفاده کنی

فرمول غلط می‌شود، چون `z[1]` باید از **x قدیمی** استفاده کند، نه x جدید.

پس:
- `tmp` real جدید را موقت نگه می‌دارد
- imaginary با x,y قدیمی حساب می‌شود
- در آخر `z[0] = tmp`

این نکته معمولاً evaluator می‌پرسد.

---

## 30) شرط Escape چرا `|z|^2 < 4.0` است؟

در کد شرط loop:

```c
(z[0] * z[0] + z[1] * z[1]) < 4.0
```

یعنی:
- `|z|^2 < 4`
- معادل `|z| < 2`

### 30.1) چرا شعاع 2؟

برای این خانواده (`z^2 + c`) اگر قدرمطلق از 2 رد شود، دنباله به سمت بی‌نهایت می‌رود (escape).

### 30.2) چرا مربع قدرمطلق و نه خود قدرمطلق؟

- `|z| = sqrt(x^2 + y^2)`
- گرفتن `sqrt` برای هر iteration کند است.
- پس مستقیم `x^2 + y^2` را با `4` مقایسه می‌کنیم.

این optimization استاندارد fractol است.

---

## 31) تفاوت دقیق سه Fractal در کد تو

## 31.1) Mandelbrot

- `z0 = 0`
- `c = pixel`

در کد:
- داخل `init_iter` چون `set != JULIA`:
  - `z[0] = 0.0`
  - `z[1] = 0.0`
- و `cx,cy` همان مختصات پیکسل می‌مانند.

## 31.2) Julia

- `z0 = pixel`
- `c = constant` (`julia_x`, `julia_y`)

در کد (`init_iter`):
- `z[0] = *cx`
- `z[1] = *cy`
- `*cx = data->julia_x`
- `*cy = data->julia_y`

یعنی:
- pixel می‌رود داخل `z`
- ثابت Julia می‌رود داخل `c`

## 31.3) Burning Ship

مثل Mandelbrot است، اما قبل از مربع کردن:
- `x = |x|`
- `y = |y|`

در کد:

```c
if (data->set == BURNING_SHIP)
{
    z[0] = fabs(z[0]);
    z[1] = fabs(z[1]);
}
```

این تغییر کوچک، شکل fractal را کاملاً عوض می‌کند.

---

## 32) Mapping کامل پیکسل ↔ صفحه مختلط

در `render`:

```c
r.sx = 4.0 / (WIDTH * zoom);
r.sy = 4.0 / (HEIGHT * zoom);
r.start_x = (-WIDTH / 2.0) * r.sx + shift_x;
r.start_y = (-HEIGHT / 2.0) * r.sy + shift_y;
cx = start_x + x * sx;
cy = start_y + y * sy;
```

### 32.1) معنی هندسی

- صفحه نمایش 800x800 است.
- ما یک «پنجره» روی صفحه مختلط می‌گیریم.
- `sx/sy` می‌گویند هر پیکسل چند واحد مختلط است.

### 32.2) نقش `zoom`

- `zoom` زیاد شود -> `sx/sy` کوچک می‌شوند -> جزئیات بیشتر (zoom in).
- `zoom` کم شود -> `sx/sy` بزرگ می‌شوند -> نمای بازتر (zoom out).

### 32.3) نقش `shift_x/shift_y`

- کل پنجره دید را روی صفحه مختلط جابه‌جا می‌کنند.
- Arrow keys دقیقاً همین‌ها را تغییر می‌دهند.

### 32.4) mapping معکوس (دانستن برای دفاع)

اگر evaluator بپرسد: «مختصات مختلط معلوم، پیکسلش کجاست؟»

از:
- `cx = start_x + x*sx` -> `x = (cx - start_x)/sx`
- `cy = start_y + y*sy` -> `y = (cy - start_y)/sy`

---

## 33) اثبات ریاضی Zoom around Mouse در کد تو

تابع: `zoom_at_point`

ایده:
- قبل از zoom، نقطه مختلط زیر mouse را ذخیره می‌کنیم: `mouse_re`, `mouse_im`.
- بعد zoom عوض می‌شود.
- بعد shift را طوری تنظیم می‌کنیم که mouse دوباره همان نقطه را نشان بدهد.

فرمول کد:

```c
mouse_re = (x - WIDTH/2.0)*scale_x + shift_x;
zoom *= factor;
shift_x = mouse_re - (x - WIDTH/2.0)*new_scale_x;
```

چرا درست است؟

بعد از update باید این برقرار باشد:
- `mouse_re = (x - WIDTH/2)*new_scale_x + new_shift_x`

پس:
- `new_shift_x = mouse_re - (x - WIDTH/2)*new_scale_x`

دقیقاً همان کد.

نتیجه:
- zoom حول cursor انجام می‌شود، نه حول مرکز.

---

## 34) رنگ‌ها: ریاضی و رفتار بصری

### 34.1) `get_color(iter)`

- اگر `iter == MAX_ITER` -> سیاه (داخل set).
- در غیر این‌صورت:
  - `t = iter / MAX_ITER`
  - RGB با polynomial نرم محاسبه می‌شوند.

چرا polynomial؟
- transition نرم‌تر می‌دهد نسبت به رنگ‌پله‌ای ساده.

### 34.2) `get_palette_color(iter, shift)`

- palette یکبار ساخته می‌شود (`static` + `ready`).
- هر بار فقط index جابجا می‌شود:
  - `i = (iter + shift) % MAX_ITER`

### 34.3) چرا modulo با correction منفی؟

در C ممکن است `%` برای منفی، نتیجه منفی بدهد.
پس:

```c
if (i < 0)
    i += MAX_ITER;
```

تا index همیشه داخل بازه معتبر باشد.

---

## 35) Parsing و Validation: دقیق و دفاع‌پذیر

## 35.1) ورودی‌های معتبر

1. `./fractol mandelbrot`
2. `./fractol burningship`
3. `./fractol burning_ship`
4. `./fractol julia`
5. `./fractol julia <x> <y>`

## 35.2) ورودی‌های نامعتبر (نمونه)

1. `./fractol`
2. `./fractol abc`
3. `./fractol julia 1`
4. `./fractol julia x y`
5. `./fractol julia 1.2.3 0`

## 35.3) `ft_atof_strict` چه چیزی را reject می‌کند؟

- کاراکتر اضافی: `12a`
- بدون رقم: `.` یا `+` یا `-`
- رشته خالی

## 35.4) چرا strict parser خوب است؟

- evaluator می‌بیند input validation واقعی داری.
- رفتار deterministic می‌ماند.

---

## 36) Function Contracts (خیلی مهم برای دفاع)

این بخش مثل قرارداد رسمی هر تابع است.

## 36.1) `init_data(t_data *data)`
- ورودی: pointer معتبر به `t_data`
- خروجی: ندارد (`void`)
- اثر: کل state را مقداردهی اولیه می‌کند
- invariant بعد از خروج:
  - pointerها `NULL`
  - zoom = 1
  - defaults Julia آماده

## 36.2) `parse_args(int ac, char **av, t_data *data)`
- ورودی: args خط فرمان
- خروجی:
  - `1` = parse موفق
  - `0` = parse ناموفق (usage چاپ شده)
- اثر: `data->set` و احتمالاً `julia_x/y` تنظیم می‌شوند

## 36.3) `setup_graphics(t_data *data)`
- خروجی:
  - `0` = موفق
  - `1` = خطا
- اثر: `mlx`, `win`, `img`, `addr` و metadata مقدار می‌گیرند

## 36.4) `render(t_data *data)`
- خروجی: ندارد
- اثر: کل buffer تصویر با وضعیت فعلی state پر می‌شود و روی window گذاشته می‌شود

## 36.5) `mouse_hook(...)`, `key_hook(...)`
- ورودی: event
- خروجی: `0`
- اثر: state تغییر می‌کند و در صورت event معتبر، `render` می‌شود

## 36.6) `destroy_data(t_data *data)`
- خروجی: ندارد
- اثر: تمام resourceهای MLX که ساخته شده‌اند آزاد می‌شوند

---

## 37) سناریوی کامل Event Timeline (مثال واقعی)

سناریو:
1. اجرا: `./fractol mandelbrot`
2. تصویر اول می‌آید.
3. کاربر wheel up می‌زند:
   - `mouse_hook` -> `zoom_at_point` -> `render`
4. کاربر `Right` می‌زند:
   - `key_hook` -> `shift_x += MOVE_STEP/zoom` -> `render`
5. کاربر `C` می‌زند:
   - `color_shift += 2` -> `render`
6. کاربر `R` می‌زند:
   - `set_default_view` -> `render`
7. کاربر ESC:
   - `close_hook` -> `destroy_data` -> `exit(0)`

اگر evaluator بگوید «Flow واقعی را بگو»، همین timeline جواب عالی است.

---

## 38) سوالات سخت evaluator + جواب پیشنهادی

### Q1) چرا `set_default_view` داخل `parse_args` صدا زده‌ای؟
A:
- چون framing پیش‌فرض هر set فرق دارد.
- بعد از اینکه set مشخص شد، دوربین اولیه متناسب با همان set تنظیم می‌شود.

### Q2) چرا از `static palette` استفاده کردی؟
A:
- برای جلوگیری از محاسبه مجدد رنگ‌ها در هر پیکسل.
- فقط یک‌بار ساخته می‌شود و frameها سریع‌تر می‌شوند.

### Q3) اگر `mlx_new_window` fail کند چه می‌شود؟
A:
- تابع `setup_graphics` خطا را detect می‌کند، cleanup می‌کند، و `1` برمی‌گرداند.
- `main` با خطا خارج می‌شود.

### Q4) چرا خروج با `return (1)` در parse/setup خطا درست است؟
A:
- چون non-zero status نشان‌دهنده failure در Unix است.

### Q5) تفاوت `button == 4` و `button == 5` چیست؟
A:
- روی Linux/X11: 4 = wheel up، 5 = wheel down.

### Q6) چرا داخل حرکت از `MOVE_STEP / zoom` استفاده کردی؟
A:
- تا حرکت screen-space تقریباً ثابت بماند و در zoom بالا کنترل دقیق‌تر شود.

### Q7) چرا `julia` بدون پارامتر هم قبول می‌کنی؟
A:
- usability بهتر است و با defaultهای معنی‌دار Julia کار می‌کند.
- همچنان `julia <x> <y>` هم پشتیبانی می‌شود.

### Q8) اگر evaluator بگوید یک کلید جدید اضافه کن چه می‌کنی؟
A:
- در `key_hook` یک branch جدید اضافه می‌کنم.
- state لازم را تغییر می‌دهم.
- `render(data)` صدا می‌زنم.

---

## 39) چک‌لیست نهایی دفاع (قبل از evaluation)

1. `norminette *.c *.h` پاس.
2. `make`, `make bonus`, `make fclean` پاس.
3. ورودی غلط usage می‌دهد.
4. هر 3 set اجرا می‌شوند.
5. zoom/move/color/close درست کار می‌کنند.
6. می‌توانی شفاهی این را بگویی:
   - state model (`t_data`)
   - parse flow
   - render mapping
   - iteration formula
   - event-driven redraw
   - cleanup path

---

## 40) پاسخ خیلی کوتاه 60 ثانیه‌ای برای «پروژه‌ات چطور کار می‌کند؟»

`fractol` من یک برنامه event-driven با MiniLibX است. در `main` اول state را init می‌کنم، args را parse می‌کنم تا set را تعیین کنم (`MANDELBROT`, `JULIA`, `BURNING_SHIP`)، بعد گرافیک را setup می‌کنم و یک render اولیه می‌زنم. در render هر پیکسل را به یک نقطه در صفحه مختلط map می‌کنم، با فرمول `z = z^2 + c` تا `MAX_ITER` تکرار می‌کنم و با شرط escape `|z|^2 >= 4` تشخیص می‌دهم داخل/خارج set است. تعداد iteration را به رنگ تبدیل می‌کنم. بعد با event hooks، scroll برای zoom around mouse، arrow برای حرکت view، C/V برای color shift و ESC/X برای خروج تمیز داریم. cleanup کامل در `destroy_data` انجام می‌شود.

---

## 41) اگر evaluator تغییر کوچک بخواهد، چطور سریع انجام بدهی؟

### مثال 1: «MAX_ITER را runtime قابل تغییر کن»
- یک فیلد جدید به `t_data` اضافه کن (مثلاً `max_iter`).
- در `init_data` مقدار اولیه بده.
- در `key_hook` با `+/-` تغییر بده.
- در `get_point_iter` به‌جای `MAX_ITER` از `data->max_iter` استفاده کن.

### مثال 2: «کلید عوض کردن set در لحظه»
- در `key_hook` روی key خاص:
  - `data->set` را cycle کن.
  - `set_default_view(data)`
  - `render(data)`

### مثال 3: «سرعت zoom را تغییر بده»
- `ZOOM_FACTOR` را عوض کن یا runtime variable کن.

---

## 42) جمع‌بندی نهایی خیلی مهم

اگر این 5 مورد را بلد باشی، تقریباً هر سوال evaluator را جواب می‌دهی:

1. **State**: چه چیزهایی داخل `t_data` است و چرا.
2. **Parse**: چطور input معتبر/نامعتبر تشخیص می‌دهی.
3. **Math**: فرمول `z^2 + c` و شرط escape `|z|^2 < 4`.
4. **Render**: mapping پیکسل به صفحه مختلط + loopها.
5. **Events/Cleanup**: چطور interaction و خروج تمیز انجام می‌شود.

تو الآن ساختار فنی لازم را داری؛ با این سند می‌توانی دفاع را خیلی مطمئن انجام بدهی.

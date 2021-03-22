____
# 3.14 of Satan

Тема на киберфоруме:
[www.cyberforum.ru](https://www.cyberforum.ru/contests/thread2787489.html)
____
## Общий комментарий:

:one: Был использован алгоритм `Брента — Саламина`. Программа сначала писалась под `MSYS10 + GMP`, пока не стало ясно, что точности long в `mpf_set_default_prec(AMOUNT_BIT)` для решения задачи не достаточно. Поэтому было принято решение переползти на MPIR, который очень хорошо дружит с VC++. Отсюда возник тандем `VC++2019 + MPIR`

:two: Конфиг `Config.txt` настроен на рендер `числа Пи` такого размера, которое могло бы содержать ВСЕ вхождения циферных последовательностей длиной `8`. Чтобы сгенерирвать число ПИ требуемого размера,
предположительно достаточно иметь в конфиге `AMOUNT_ITERATIONS  = 29`.

:three: После окончания генерации Пи и записи его в файл автоматичеcки стартует билдинг словаря Сатаны, предположительно он должен быть полным, то есть содержать ровно `100'000'000` записей согласно условию задачи.

:four: После генерации словаря будет запущен ручной тест поиска пароля по слову Сатаны. Пароль ищется в файле без его загрузки в память способом адресации по содержанию (или другими словами - способом индексации массива), т.е. с алгоритмической сложностью **O(1)** Внимание, поиск возможен только, если будет сгенерирован **`ПОЛНЫЙ(!) словарь`** содержащий ВСЕ записи, иначе программа предупредит вас об этом.

:eight_pointed_black_star: в код внедрен хак(244 недостающих записей) который дополняет словарь до `ПОЛНОГО для 29 итераций`.
____
## Как оно выглядит:

![Screenshot in game 1](!/screenshot_01.png)
![Screenshot in game 1](!/screenshot_02.png)
____
## Файловая схема:    

                            .----------------------. 
                            |       mylib.h        |
                            '----------------------'
                                        ^
                                        |
                                        |
                            .----------------------.
                            |       Config.h       |
                            '----------------------'
                                ^               ^
                                |               |
                                |               |
                |----------------------| |----------------------|
    START------>|       main.cpp       | | Tools_render_vob.cpp |
                |----------------------| |----------------------|
____
## Лог теста:

**Конфигурация:** `[CPU Celeron G3900 SkyLake] + [RAM 16 Gb Dual] + [HDD-1000-64]`

```
///-------------------------|
/// Загруженный конфиг.     |
///-------------------------:
 cfg.AMOUNT_ITERATIONS = 29
 cfg.is_skip_hand_test = 1
            cfg.LENGTH = 8
cfg.is_rewrite_pi_file = 1
        cfg.is_visible = 1

///-----------------------------|
/// Render числа PI.            |
///-----------------------------:
AMOUNT_BIT = 4294967296
Всего итераций: 30
Time operation: 7:40:41

///-------------------------|
/// Запись на диск.         |
///-------------------------:
Pi is Ready! (1292913988 bytes)
Time operation: 0:44:59

///-----------------------------|
/// Render словаря Сатаны.      |
///-----------------------------:
LENGTH = 8
FIND: 99999756 (from:100000000)

Efficiency: 99.9998%
Time operation: 1:10:25
Последнее найденное число: 30470270 на позиции 1291752655
```

Итого, не  хватает ещё `244` числа.
Возможно увеличние **`cfg.AMOUNT_ITERATIONS`** до **`30`** будет достаточно.

____
## Внимание: хак!

Для количества итераций = `29` недостающие `244 пассворда` будут догружены из зашитого в *.exe массива.
Сам файл с константами [тут: key_pass.inl](https://github.com/BDOTimer/Satana/blob/main/Project-VC%2B%2B2019/Satan/Sources/key_pass.inl)    
Даннай файл был сгененирован из данных любезно предоставленных [outoftime](https://www.cyberforum.ru/post15350704.html).    
    
Итого:
![Screenshot in game 1](!/screenshot_03.png)    
Размер словаря 953 Mb..
____
## Полезные ссылки:

[Pi - Chudnovsky](https://www.craig-wood.com/nick/articles/pi-chudnovsky/)

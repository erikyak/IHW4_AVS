
1. ФИО: Яковлев Эрик денисович
2. Группа: БПИ239
3. - Вариант: 4
   - Текст задачи: Неправильные пчелы, подсчитав в конце месяца убытки от наличия в лесу Винни Пуха, решили разыскать его и наказать в назидание всем другим любителям сладкого.
   Для поисков медведя они поделили лес на участки, каждый из которых прочесывает одна стая неправильных пчел. В случае нахождения медведя на своем участке стая проводит
   показательное наказание и возвращается в улей. Если участок прочесан, а Винни-Пух на нем не обнаружен, стая также возвращается в улей.
 Там она получает информацию об еще неисследованных участках и снова улетает. Требуется создать многопоточное приложение, моделирующее действия пчел.
 При решении использовать парадигму «портфель задач». Каждая стая пчел — отдельный поток.
4. Метод решения задачи:  
4.1. Сценарий: В задаче рои пчёл ищут Винни-Пуха, который спрятался в одной из секций леса. Каждый рой проверяет секции по очереди.
    Если один из роев находит Винни-Пуха, он сигнализирует остальным и завершает поиск.  
  4.2. Роли и объекты:  
    4.2.1. Рои пчёл: Многопоточные сущности, которые обрабатывают секции леса.  
    4.2.2. Секции леса: Очередь задач, по которым ведётся поиск.  
    4.2.3. Винни-Пух: Объект, спрятанный в одной из секций, которого нужно найти.  
4.3. Синхронизация: Мьютекс используется для защиты доступа к очереди задач, а условная переменная — для уведомления о новых задачах.  




<details>
  <summary>Отчет</summary>

## На 4-5 баллов
### Модель параллельных вычислений  
Используется POSIX Threads:
- Мьютекс (pthread_mutex_t) синхронизирует доступ к очереди.
- Условная переменная (pthread_cond_t) позволяет потокам ждать новых задач.  
Каждый поток обрабатывает секции из очереди и завершает работу при обнаружении Винни-Пуха.
### Входные данные
Ввод с консоли:
- Размер леса (количество секций).
- Количество роев пчёл.
- Номер секции, где спрятан Винни-Пух.  
Пример ввода:
```
What is the size of the forest? (enter a positive number):  10  
How many bee swarms are there? (enter a positive number): 3  
Where are you hiding? (choose a section between 1 and 10): 5
```
### Вывод программы
Вывод отображает ключевые события:
```
Bee swarm 1 is searching section 1
Bee swarm 1 did not find Winnie the Pooh in section 1 and returned to the hive
Bee swarm 3 is searching section 3
Bee swarm 3 did not find Winnie the Pooh in section 3 and returned to the hive
Bee swarm 2 is searching section 2
Bee swarm 2 did not find Winnie the Pooh in section 2 and returned to the hive
Bee swarm 1 is searching section 4
Bee swarm 1 did not find Winnie the Pooh in section 4 and returned to the hive
Bee swarm 2 is searching section 6
Bee swarm 2 did not find Winnie the Pooh in section 6 and returned to the hive
Bee swarm 3 is searching section 5
Bee swarm 3 found and punished Winnie the Pooh in section 5
Bee swarm 1 is searching section 7
Bee swarm 1 did not find Winnie the Pooh in section 7 and returned to the hive
Bee swarm 2 is searching section 8
Bee swarm 2 did not find Winnie the Pooh in section 8 and returned to the hive
All bee swarms have returned to the hive.
Winnie the Pooh was found by bee swarm 3 in section 5.
Bee swarm 1 is searching section 3
Bee swarm 2 found and punished Winnie the Pooh in section 5
All bee swarms have returned to the hive.
```

## На 6-7 баллов


## На 8 баллов



## На 9 баллов


## На 10 баллов

</details>

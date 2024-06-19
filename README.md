# Арисова Елизавета Ивановна, БПИ227
ИДЗ 2, вариант 14, выполнено на оценку 4-5
# Условие:
Вторая Задача о магазине (забывчивые покупатели). В магазине работают два отдела, каждый отдел обладает уникальным ассортиментом. В каждом отделе работает один продавец. В магазин ходят исключительно забывчивые покупатели, поэтому каждый покупатель носит с собой список из K товаров, которые желает купить. Покупатель приобретает товары точно в том порядке, в каком они записаны в его списке. Число товаров в списке от одного до пяти. При этом товары в списке расположены в случайном порядке, что заставляет покупателя многократно переходить от отдела к отделу, если это требуется для совершения покупок. Продавец может обслужить только одного покупателя за раз. Покупатель, вставший в очередь, засыпает пока не дойдет до продавца. Продавец засыпает, если в его отделе нет покупателей, и просыпается, 14 если появится хотя бы один. Создать многопроцессное приложение, моделирующее работу магазина в течение рабочего дня.
# Алгоритм решения:
Каждого из покупателей обозначает отдельный процесс. Все они запускаются в одном файле с помощью команды fork(). Количество покупателей находятся в переменной customers.
В начале создаются списки товаров для каждого покупателя:
![image](https://github.com/in-the-neon-sunrise/OS_ihw2/assets/64023022/78ad0ea1-854c-42ee-9478-fd157c385b54)
Далее каждый покупатель идет по своему списку:
![image](https://github.com/in-the-neon-sunrise/OS_ihw2/assets/64023022/05c5446d-c8e7-4382-9cc0-29c22d165ee4)
В программе предусмотрено ее корректное завершение с удалением разделяемой памяти и семафоров после того, как все покупатели закончат свои списки.

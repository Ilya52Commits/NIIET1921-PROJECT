## Документации и проекты на микроконтроллере "НИИЭТ" 1921ВК035
Здесь собраны документации работы с отечественным микроконтроллером компании АО "НИИЭТ" - 1921ВК035. Также собраны проекты для общего понимания того, как работать с отечественным микроконтроллером.

[Создание проекта в Keil](#Создание-проекта-в-Keil)
## Создание проекта в Keil 
В данной документации применяется отладочная плата АО "НИИЭТ" КФДЛ.441461.018Б и среда разработки Keil. 
Создать новый проект в Keil для работы с микроконтроллером 1921ВК035 можно использовать два способа: использовать шаблон из репозитория НИИЭТ и настроить проект вручную.
### Создание проекта с помощью шаблона
Для данного способа необходим локальный клонированный репозиторий K1921VKx_SDK:
  - GitFlic: https://gitflic.ru/project/niiet/k1921vkx_sdk
  - Bitbucket: https://bitbucket.org/niietcm4/k1921vkx_sdk/src/develop/

Для создания проекта с помощью шаблона необходимо зайти в директорию templates, где присутсвуют примеры проектов для трёх микроконтроллеров от "НИИЭТ": 1921ВК01Т, 1921ВК028, 1921ВК035. В данном примере необходим один из двух шаблонов: k1921vk035-bare и k1921vk035-plib035. В первом шаблоне присутствуют только базовые файлы для сборки проекта, а во втором шаблоне базовые файлы с библиотеками периферии.
Для примера будет использован шаблон k1921vk035-plib035. В уже клонированном репозитории необходимо зайти в данный шаблон и скопировать содержимое в папку для вашего проекта. 

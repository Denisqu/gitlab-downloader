### Сейчас
1) Заменить gtest на QTest (https://doc.qt.io/qt-6.5/qtest-tutorial.html) и QCoroTest (https://doc.qt.io/qt-6.5/qtest-tutorial.html) 
1) Написать тесты для Gitlab::ReplyHandler
1) Выделить интерфейс: IRequestFormatter
1) Написать тесты для Gitlab::RequestFormatter
1) Переписать Gitlab::Handler, чтобы его можно было протестировать (возможно с помощью mock-объектов)
1) Написать логику для gitlab_handler.cpp с помощью Coroutines, TDD [ ]
1) Написать логику для db_manager с помощью TDD (сделать как здесь: https://github.com/Denisqu/qt-cxx-reference-projects/blob/main/projects/ch03-gallery-core/gallery-core/DatabaseManager.cpp) [ ]

### Потом
1) Почистить CMake файлы от мусора. [ ] 
1) Сделать наброс UI на FluentUI QML [ ]

### Возможно уже не нужно
none

### Полезные Ресурсы для проекта
1) QML Book: https://distracted-dijkstra-f5d508.netlify.app/

### Сделано
1) Выделить интерфейс: IReplyHandler 
1) Отделить RequestFormatter в отдельную сущность, чтобы было легче тестировать (TDD) [+]
1) Сделать Model+View заготовку для будущей реализации главного экрана [+] 
1) Решить проблему с тем, что лог не пишется от объектов в другом потоке. [+]
1) Убрать asyncfuture [+]
1) Собрать Qt 6.5.3  из исходников [+]
1) Redirect qDebug to SimpleLogger (https://www.francescmm.com/logging-with-qt/) [+]
1) Добавить новый логгер [+]
1) Успешно собрать MSVC_CXX20 + QCoro + CoroutineTest [+]
1) Линкануться к QCoro https://github.com/danvratil/qcoro [+]
1) Обновить Qt до 6.5 LTS [+]
1) Разбить проект на 3 сабдиректории с CMake проектами: core, widgets-impl, qml-impl[+]
1) Сделать реализацию наброска UI [+] 
1) Поправить build_and_run.py, чтобы билд собирался в правильном месте и без мусора [+]
1) Добавить g-test, g-mock в проект [+]
1) Добавить Logger в проект [+]

### Отмена
1) Добавить timeout для QNetworkRequest: void QNetworkRequest::setTransferTimeout(int timeout = DefaultTransferTimeoutConstant  [-]
1) Перенести логгер в свой поток. [-]
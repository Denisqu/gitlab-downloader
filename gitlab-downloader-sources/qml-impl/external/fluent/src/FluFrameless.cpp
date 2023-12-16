#include "FluFrameless.h"

#include <QGuiApplication>
#include <QOperatingSystemVersion>
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <windows.h>
static inline QByteArray qtNativeEventType()
{
    static const auto result = "windows_generic_MSG";
    return result;
}
#endif

FramelessEventFilter::FramelessEventFilter(QQuickWindow* window){
    _window = window;
    _current = window->winId();
}

bool FramelessEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, QT_NATIVE_EVENT_RESULT_TYPE *result){
#ifdef Q_OS_WIN
    if ((eventType != qtNativeEventType()) || !message || !result || !_window) {
        return false;
    }
    const auto msg = static_cast<const MSG *>(message);
    const HWND hWnd = msg->hwnd;
    if (!hWnd) {
        return false;
    }
    const qint64 wid = reinterpret_cast<qint64>(hWnd);
    if(wid != _current){
        return false;
    }
    const UINT uMsg = msg->message;
    if (!msg || !msg->hwnd)
    {
        return false;
    }
    if(uMsg == WM_WINDOWPOSCHANGING){
        WINDOWPOS* wp = reinterpret_cast<WINDOWPOS*>(msg->lParam);
        if (wp != nullptr && (wp->flags & SWP_NOSIZE) == 0)
        {
            wp->flags |= SWP_NOCOPYBITS;
            *result = DefWindowProc(msg->hwnd, msg->message, msg->wParam, msg->lParam);
            return true;
        }
        return false;
    }else if(uMsg == WM_NCCALCSIZE){
        *result = WVR_REDRAW;
        return true;
    }else if(uMsg == WM_NCPAINT){
        *result = WVR_REDRAW;
        return false;
    }
    return false;
#endif
    return false;
}

FluFrameless::FluFrameless(QObject *parent)
    : QObject{parent}
{
}

void FluFrameless::classBegin(){
}

void FluFrameless::updateCursor(int edges){
    switch (edges) {
    case 0:
        _window->setCursor(Qt::ArrowCursor);
        break;
    case Qt::LeftEdge:
    case Qt::RightEdge:
        _window->setCursor(Qt::SizeHorCursor);
        break;
    case Qt::TopEdge:
    case Qt::BottomEdge:
        _window->setCursor(Qt::SizeVerCursor);
        break;
    case Qt::LeftEdge | Qt::TopEdge:
    case Qt::RightEdge | Qt::BottomEdge:
        _window->setCursor(Qt::SizeFDiagCursor);
        break;
    case Qt::RightEdge | Qt::TopEdge:
    case Qt::LeftEdge | Qt::BottomEdge:
        _window->setCursor(Qt::SizeBDiagCursor);
        break;
    }
}

bool FluFrameless::eventFilter(QObject *obj, QEvent *ev){
    if (!_window.isNull() && _window->flags()& Qt::FramelessWindowHint) {

        static int edges = 0;
        const int margin = 8;
        switch (ev->type()) {
        case QEvent::MouseButtonPress:
            if(edges!=0){
                updateCursor(edges);
                _window->startSystemResize(Qt::Edges(edges));
            }
            break;
        case QEvent::MouseButtonRelease:
            edges = 0;
            updateCursor(edges);
            break;
        case QEvent::MouseMove: {
            if(_window->visibility() == QWindow::Maximized || _window->visibility() == QWindow::FullScreen){
                break;
            }
            if(_window->width() == _window->maximumWidth() && _window->width() == _window->minimumWidth() && _window->height() == _window->maximumHeight() && _window->height() == _window->minimumHeight()){
                break;
            }
            QMouseEvent *event = static_cast<QMouseEvent*>(ev);
            QPoint p =
#if QT_VERSION < QT_VERSION_CHECK(6,0,0)
                event->pos();
#else
                event->position().toPoint();
#endif
            if(p.x() >= margin && p.x() <= (_window->width() - margin) && p.y() >= margin && p.y() <= (_window->height() - margin)){
                if(edges != 0){
                    edges = 0;
                    updateCursor(edges);
                }
                break;
            }
            edges = 0;
            if ( p.x() < margin ) {
                edges |= Qt::LeftEdge;
            }
            if ( p.x() > (_window->width() - margin) ) {
                edges |= Qt::RightEdge;
            }
            if ( p.y() < margin ) {
                edges |= Qt::TopEdge;
            }
            if ( p.y() > (_window->height() - margin) ) {
                edges |= Qt::BottomEdge;
            }
            updateCursor(edges);
            break;
        }
        default:
            break;
        }
    }
    return QObject::eventFilter(obj, ev);
}

void FluFrameless::componentComplete(){
    auto o = parent();
    while (nullptr != o) {
        _window = (QQuickWindow*)o;
        o = o->parent();
    }
    if(!_window.isNull()){
        _window->setFlag(Qt::FramelessWindowHint,true);
        _window->installEventFilter(this);
#ifdef Q_OS_WIN
        _nativeEvent =new FramelessEventFilter(_window);
        qApp->installNativeEventFilter(_nativeEvent);
        HWND hwnd = reinterpret_cast<HWND>(_window->winId());
        ULONG_PTR cNewStyle = GetClassLongPtr(hwnd, GCL_STYLE) | CS_DROPSHADOW;
        SetClassLongPtr(hwnd, GCL_STYLE, cNewStyle);
        SetWindowPos(hwnd,nullptr,0,0,0,0,SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOSIZE |SWP_FRAMECHANGED);
#endif
    }
}

FluFrameless::~FluFrameless(){
    if (!_window.isNull()) {
        _window->setFlag(Qt::FramelessWindowHint,false);
        _window->removeEventFilter(this);
#ifdef Q_OS_WIN
        qApp->removeNativeEventFilter(_nativeEvent);
#endif
    }
}

#include "chip8_window.hpp"

#include <QThread>
#include <QKeyEvent>

#include "chip8_worker.hpp"

Chip8Window::Chip8Window(QWidget *parent) : QWidget(parent) 
{ 
    Chip8Worker* chip8_worker = new Chip8Worker(chip8,this);
    QThread* thread = new QThread(this);

    worker->moveToThread(thread);

    connect(thread, &QThread::started, worker, &Worker::start);
    connect(worker, &Worker::finished, thread, &QThread::quit);

    connect(worker, &Worker::finished, worker, &Worker::deleteLater);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);

    connect(worker, &Worker::update, this, &Chip8Window::update);

    thread->start();
}

void Chip8Window::update() 
{
    chip8_display.updateScreen(chip8.getScreenData());
    chip8_display.update();

}

void Chip8Window::keyPressEvent(QKeyEvent *event)
{ 

} 

void Chip8Window::keyReleaseEvent(QKeyEvent *event) 
{ 

}

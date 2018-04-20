#ifndef COMMAND_H
#define COMMAND_H

#include <QObject>
#include <QPen>
#include <vector>
#include "textedit.h"

class Command
{
public:
    enum CommandType{
        NONE,
        DRAW_CIRCLE,
        DRAW_RECTANGLE,
        DRAW_ARROW,
        DRAW_BROKEN_LINE,
        DRAW_CURVE,
        DRAW_TEXT,
    };
public:
    Command(CommandType type) : type_(type) { }
    Command(CommandType type, const QPen& pen)
        : type_(type), pen_(pen)
    { }
    Command(CommandType type, const QPen& pen, const QPoint& pos, bool is_fill, const std::vector<QPoint>& points)
        : type_(type), pen_(pen), position_(pos), is_fill_(is_fill), points_(points)
    { }

    inline void type(CommandType t) { type_ = t; }
    inline CommandType type() const  { return type_; }

    inline void position(QPoint pos) { position_ = pos; }
    inline QPoint position() const { return position_; }

    inline void pen(const QPen& pen) { pen_ = pen; }
    inline QPen pen() const { return pen_; }

    inline void setFill(bool fill) { is_fill_ = fill; }
    inline bool isFill() const { return is_fill_; }

    inline void points(const std::vector<QPoint>& ps) { points_ = ps; }
    inline std::vector<QPoint> points() const { return points_; }

    inline void widget(QWidget * widget) { widget_ = widget; }
    inline QWidget * widget() const { return widget_; }

private:
    CommandType type_{ NONE };
    QPoint position_{ 0, 0 };
    QPen pen_;
    bool is_fill_ = false;
    std::vector<QPoint> points_;
    QWidget * widget_ = nullptr;
};

class CommandStack : public QObject
{
    Q_OBJECT

public:
    inline void push(const Command& command)
    {
        if(stack_.empty())
            emit empty(false);

        stack_.push_back(command);

        emit changed();
        emit pushed();
    }

    inline void pop()
    {
        stack_.pop_back();

        if(stack_.empty())
            emit empty(true);

        emit changed();
        emit poped();
    }


    inline size_t size() const { return stack_.size(); }

    inline Command back() const { return stack_.back(); }

    inline std::vector<Command> commands() const { return stack_; }

    inline bool empty() const { return stack_.empty(); }

signals:
    void changed();
    void pushed();
    void poped();

    void empty(bool);

public slots:
    inline void clear() { stack_.clear(); }

private:
    std::vector<Command> stack_;
};


#endif // COMMAND_H
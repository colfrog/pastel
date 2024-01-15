#include <QWidget>
#include <QTimer>

#include "board.h"

class BoardView : public QWidget {
	Q_OBJECT

public:
	BoardView(QWidget *parent = nullptr);

	void set_square_size();

public slots:
	void step();

protected:
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	Board m_board;
	QTimer *m_timer;

	int m_square_size;
};

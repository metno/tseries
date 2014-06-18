/*
 * ClearLineEdit.h
 *
 *  Created on: Jun 18, 2014
 *      Author: juergens
 */

/****************************************************************************
**
** Copyright (c) 2007 Trolltech ASA <info@trolltech.com>
**
** Use, modification and distribution is allowed without limitation,
** warranty, liability or support of any kind.
**
****************************************************************************/

#ifndef CLEARLINEEDIT_H
#define CLEARLINEEDIT_H

#include <QLineEdit>

class QToolButton;

class ClearLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    ClearLineEdit(QWidget *parent = 0);

protected:
    void resizeEvent(QResizeEvent *);

private slots:
    void updateCloseButton(const QString &text);

private:
    QToolButton *clearButton;
};

#endif // LIENEDIT_H





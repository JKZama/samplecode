#pragma once // Same as #ifdef ... / #define ... / #endif
#include<QFrame>
#include<QHBoxLayout>
#include<QLineEdit>
#include<QPushButton>
#include "entry.h"
/* A frame (set of widgets) that adds new phone numbers to the phonebook. */
class newNumber : public QFrame
{
  Q_OBJECT

  public:
    newNumber();

  signals:
    void addEntry(Entry* e);
  private slots:
    void addPress();

  private:
    QLineEdit* nameEdit;
    QLineEdit* numberEdit;
    QPushButton* addButton;
};

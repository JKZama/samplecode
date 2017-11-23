//Jesse Zamazanuk
//Function for adding new numbers to the phonebook in the QT app
#include "newNumber.h"
newNumber::newNumber()
{
  nameEdit = new QLineEdit;
  numberEdit = new QLineEdit;
  addButton = new QPushButton("Add Number");
  QString* name= new QString("Name");
  QString* number= new QString("Phone Number");
  nameEdit->setPlaceholderText(*name);
  numberEdit->setPlaceholderText(*number);
  connect(addButton, SIGNAL(clicked()), this, SLOT(addPress()));

  QLayout* layout = new QHBoxLayout;
  layout->addWidget(nameEdit);
  layout->addWidget(numberEdit);
  layout->addWidget(addButton);

  setLayout(layout);
}

void newNumber::addPress()
{
  Entry* numPointer = new Entry(nameEdit->text(), numberEdit->text());
  emit addEntry(numPointer);
  nameEdit->clear();
  numberEdit->clear();
}

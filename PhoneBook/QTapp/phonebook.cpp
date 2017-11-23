//Jesse Zamazanuk
//This file defines the majority of functions for the QT phonebook ap

#include"phonebook.h"

PhoneBook::PhoneBook()
{
  // Set Up Layout
  newnum = new newNumber;

  connect(newnum, SIGNAL(addEntry(Entry*)), this, SLOT(newEntry(Entry*)));
  layout = new QVBoxLayout;
  layout->addWidget(newnum);

  QWidget* main = new QWidget;
  main->setLayout(layout);
  setCentralWidget(main);

  openAction = new QAction("&Open", this);
  connect(openAction, SIGNAL(triggered()), this, SLOT(open()));

  saveAction = new QAction("&Save", this);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

  quitAction = new QAction("&Quit",this);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));

  menuBar()->setNativeMenuBar(false);

  fileMenu = menuBar()->addMenu("&File");
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(quitAction);

  modified = false;
}

void PhoneBook::newEntry(Entry* e){
  layout->addWidget(e);
  entries.push_back(e);
  modified = true;
}
void PhoneBook::save()
{
  QString fileName = QFileDialog::getSaveFileName(
      this, // Parent
      "Save File", // Dialog Title
      "", // Directory
      "All Files (*)" // File Type Filters
    );

  if(fileName != "")
  {
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly))
    {
      QMessageBox::critical(
          this, // Parent
          "Error", // Dialog Title
          "Could not write to file" // Dialog Text
        );
    }
    else
    {
      QTextStream out(&file);
      out<<serialize();
      modified = false;
      file.close();
    }
  }
}

void PhoneBook::open()
{
  QString fileName = QFileDialog::getOpenFileName(
      this, // Parent
      "Open File", // Dialog Title
      "", // Directory
      "All Files (*)" // File Type Filters
    );

  if(fileName != "")
  {
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
      QMessageBox::critical(
          this, // Parent
          "Error", // Dialog Title
          "Could not open file" // Dialog Text
        );
    }
    else
    {
      QTextStream in(&file);
      // TODO: Read data in (readAll() returns a QString with all text)
      deserialize(in.readAll());
      file.close();

    }
  }
}

void PhoneBook::quit()
{
  // TODO: Prompt to quit if there is unsaved data
  if(modified==true){
    QMessageBox messageBox;
    messageBox.setWindowTitle("Quit?");
    messageBox.setText("This phonebook has been modified. Do you want to save before quitting?");
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);
    int box = messageBox.exec();
    if(box == QMessageBox::Discard)
    {
      qApp->quit();
    }
    else if(box == QMessageBox::Save){
      save();
      qApp->quit();
    }
  }
  else{
    qApp->quit();
  }

}

QString PhoneBook::serialize()
{
  QString data = "";
  for(unsigned int i = 0; i < entries.size(); i++)
  {
    Entry* e = entries[i];
    data.append(e->getName() + ";" + e->getNumber() + "\n");
  }

  return data;
}

void PhoneBook::deserialize(QString data)
{
  // Clear all the phone numbers
  for(unsigned int i = 0; i < entries.size(); i++)
  {
    layout->removeWidget(entries[i]);
    delete entries[i];
  }
  entries.clear();

  QStringList split = data.split("\n",QString::SkipEmptyParts);
  for(int i = 0; i < split.size(); i++)
  {
    QStringList vals = split[i].split(";");

    QString name = vals[0];
    QString number = vals[1];
    Entry* e = new Entry(name, number);
    newEntry(e);

  }
}

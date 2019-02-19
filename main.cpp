#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDir>

#define WORKPATH "C:\\Users\\Robert\\Desktop\\Cplusplus\\GetVariable"

typedef struct {
    QString varName;
    QString varType;
    int varId;
} T_variable;

int main()//int argc, char *argv[])
{
    QString strTemp, strInput, strTypeVar, strIdVar, strNameVar;
    bool bFound;

    QFile readf("dl_gsw.ini");
    QString strWorkPath = WORKPATH;
    T_variable structVar;

    if (!QDir(strWorkPath).exists()) {
        qDebug() << (strWorkPath + " not exist") << endl;
        return 1;
    }

    QDir::setCurrent(strWorkPath);

    QString strFullPath = QDir::current().path() + "/" + readf.fileName();

    if (readf.open(QFile::ReadOnly)) {
        if (!readf.exists()) {
            qDebug() << strFullPath + "ini file not exist";
            return 1;
        }
    } else {
        qDebug() << strFullPath + " ini file can't open";
        return 1;
    }

    QTextStream s(stdin);
    QTextStream fil(&readf);

    do
    {

        qInfo() << endl << "Please enter name to search, or 0 to exit: ";

        //read variable name from keyboard
        structVar.varName = s.readLine();

        if (structVar.varName == "0")
            break;

        //prepare initial values
        bFound = false;
        strInput = structVar.varName;
        structVar.varType = "";
        strIdVar = "";
        fil.seek(0); //move position of file to begin

        //begin read ini file from start position
        while (!fil.atEnd()) {
            strInput = fil.readLine();

            strTemp = strInput;

            //search variable type
            strInput.replace('[', nullptr); //delete [ from string
            if (strTemp != strInput) { //found type of variable. if strings not equal that means was found [. This mean that line from file is line for type variable.
                structVar.varType = strInput.replace(']', nullptr);
                continue; //read next line from file
            }

            //search variable name
            if (strInput.contains(structVar.varName)) { // found
                bFound = true;
                int iPos = strInput.indexOf("=", 0);

                if (iPos) { //found "=", if iPos = 0 that means not found
                    try {
                        strIdVar = strInput.mid(iPos + 1); //take substring after sign "="
                        structVar.varId = strIdVar.toInt();
                    }
                    catch (std::exception &e) {
                        qDebug() << e.what();
                    }
                    break;
                }
            }

        }

        //Error: if not found variable name or not found sign "=" or not found line with "[...]"
        if (!bFound || strIdVar == "" || structVar.varType == "" ) {
            qDebug() << structVar.varName;
            qDebug() << "Can't find variable '" + structVar.varName + "' in file 'dl_gsw.ini'";
        } else {
            qDebug() << structVar.varName;
            qDebug() << "Variable '" + structVar.varName + " have type '" + structVar.varType + "' and number '" + QString::number(structVar.varId) + "'";
        }

    } while (strInput != "0");

    qDebug() << "Good bay!";
    readf.close();
    return 0;
}


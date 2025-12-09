#ifndef COMPRESSPAGE_H
#define COMPRESSPAGE_H

#include <QWidget>
#include <QString>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
// Note: You may need to update the .ui file name in the project if it was MinifyingPage.ui
namespace Ui { class CompressPage; } 
QT_END_NAMESPACE

class CompressPage : public QScrollArea
{
    Q_OBJECT

public:
    explicit CompressPage(QWidget *parent = nullptr);
    ~CompressPage();

private slots:
    void onBackToOperations();
    void onBrowseFile();
    
    void onCompressXML(); 
    void onDownload();

private:
    Ui::CompressPage *ui;
    QString inputXML;
    QString outputXML;
    QString currentFilePath;
    qint64 originalSize;
    // Renamed size variable
    qint64 compressedSize; 

    void updateStatistics();
    void updateOutputVisibility();
    void showMessage(const QString& message, bool isError = false);
};

#endif
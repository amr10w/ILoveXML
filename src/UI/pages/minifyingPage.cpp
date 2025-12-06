#include "minifyingPage.h"
#include "ui_minifyingPage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QScrollBar>

MinifyingPage::MinifyingPage(QWidget *parent)
    : QScrollArea(parent)
    , ui(new Ui::MinifyingPage)
    , originalSize(0)
    , minifiedSize(0)
{
    ui->setupUi(this);
    
    // Initially hide output section and stats
    ui->statsWidget->setVisible(false);
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
    
    // Set placeholder
    ui->inputTextEdit->setPlaceholderText("Paste your XML content here...");
    
    // Connect signals
    connect(ui->backButton, &QPushButton::clicked, this, &MinifyingPage::onBackToOperations);
    connect(ui->browseButton, &QPushButton::clicked, this, &MinifyingPage::onBrowseFile);
    connect(ui->minifyButton, &QPushButton::clicked, this, &MinifyingPage::onMinifyXML);
    connect(ui->downloadButton, &QPushButton::clicked, this, &MinifyingPage::onDownload);
}

MinifyingPage::~MinifyingPage()
{
    delete ui;
}

void MinifyingPage::onBackToOperations()
{
    this->close();
}

void MinifyingPage::onBrowseFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Browse XML File"),
        QString(),
        tr("XML Files (*.xml);;All Files (*)")
    );
    
    if (fileName.isEmpty())
        return;
    
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot open file: " + fileName);
        return;
    }
    
    QTextStream in(&file);
    inputXML = in.readAll();
    file.close();
    
    currentFilePath = fileName;
    ui->inputTextEdit->setPlainText(inputXML);
    
    // Hide output when new file is loaded
    ui->statsWidget->setVisible(false);
    ui->outputLabel->setVisible(false);
    ui->outputTextEdit->setVisible(false);
    ui->downloadButton->setVisible(false);
    
}

void MinifyingPage::onMinifyXML()
{
    inputXML = ui->inputTextEdit->toPlainText();
    
    if (inputXML.trimmed().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please provide XML content first!");
        return;
    }
    
    try {
        // Calculate original size
        originalSize = inputXML.toUtf8().size();
        
        // Minify the XML
        outputXML = minifyXMLString(inputXML);
        
        // Calculate minified size
        minifiedSize = outputXML.toUtf8().size();
        
        // Display in output text edit
        ui->outputTextEdit->setPlainText(outputXML);
        
        // Update statistics
        updateStatistics();
        
        // Show output section
        updateOutputVisibility();
        
        // Scroll to output
        QScrollBar *scrollBar = ui->outputTextEdit->verticalScrollBar();
        if (scrollBar) {
            scrollBar->setValue(0);
        }
        
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to minify XML: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "Error", 
            "An unknown error occurred while minifying XML");
    }
}

QString MinifyingPage::minifyXMLString(const QString& xml)
{
    // Convert to std::string for processing
    std::string xmlContent = xml.toStdString();
    std::string result;
    
    bool insideTag = false;
    bool prevSpace = false;
    
    for (size_t i = 0; i < xmlContent.size(); ++i) {
        char c = xmlContent[i];
        
        if (c == '<') {
            insideTag = true;
            // Remove trailing space before tag
            while (!result.empty() && result.back() == ' ')
                result.pop_back();
            result += c;
        }
        else if (c == '>') {
            insideTag = false;
            result += c;
        }
        else if (insideTag) {
            // Inside tag: preserve everything except newlines/tabs
            if (c != '\n' && c != '\r' && c != '\t')
                result += c;
        }
        else {
            // Outside tag (text content)
            if (!isspace(c)) {
                result += c;
                prevSpace = false;
            }
            else if (!prevSpace) {
                // Collapse multiple spaces into one
                result += ' ';
                prevSpace = true;
            }
        }
    }
    
    return QString::fromStdString(result);
}

void MinifyingPage::updateStatistics()
{
    // Update size labels
    ui->originalSizeLabel->setText(QString::number(originalSize) + " bytes");
    ui->minifiedSizeLabel->setText(QString::number(minifiedSize) + " bytes");
    
    // Calculate reduction
    qint64 savedBytes = originalSize - minifiedSize;
    double percentage = originalSize > 0 ? 
        ((double)savedBytes * 100.0 / originalSize) : 0.0;
    
    ui->reductionLabel->setText(QString::number(percentage, 'f', 2) + "%");
}

void MinifyingPage::updateOutputVisibility()
{
    bool hasOutput = !outputXML.isEmpty();
    ui->statsWidget->setVisible(hasOutput);
    ui->outputLabel->setVisible(hasOutput);
    ui->outputTextEdit->setVisible(hasOutput);
    ui->downloadButton->setVisible(hasOutput);
}

void MinifyingPage::onDownload()
{
    if (outputXML.isEmpty()) {
        QMessageBox::warning(this, "Warning", "No minified XML to download!");
        return;
    }
    
    QString saveFileName = QFileDialog::getSaveFileName(
        this,
        tr("Save Minified XML"),
        "minified.xml",
        tr("XML Files (*.xml);;All Files (*)")
    );
    
    if (saveFileName.isEmpty())
        return;
    
    QFile outFile(saveFileName);
    if (!outFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Cannot save file!");
        return;
    }
    
    QTextStream out(&outFile);
    out << outputXML;
    outFile.close();
    
}

void MinifyingPage::showMessage(const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, "Error", message);
    } 
}
#include "postsearch.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QTextStream>
#include <QPixmap>
#include <QScrollArea>
// #include <QGraphicsDropShadowEffect> // Start using effects if needed, but styling is mostly QSS
// #include "../../core/SearchTopic.h" 
#include "../../core/SearchWord.h" 
#include "../../core/SearchTopic.h" 

// Theme colors
#define ORANGE_THEME "#FF6D1F"
#define LIGHT_ORANGE_BG "#FFF3E0" // Lighter orange for browse button background
#define PAGE_BG "#e5e7eb" // Standard gray background
#define TEXT_COLOR "#1f2937" // Dark text color

PostSearch::PostSearch(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(900, 750);
    setWindowTitle("Post Search");

    // Global styling
    setStyleSheet(QString(
        "QWidget { background-color: %1; font-family: 'Segoe UI', sans-serif; }"
        "QMessageBox { background-color: white; }"
    ).arg(PAGE_BG));

    setupUI();

    connect(postsearchByTopics, &QPushButton::clicked, this, &PostSearch::on_searchByTopics_clicked);
    connect(postsearchByWords, &QPushButton::clicked, this, &PostSearch::on_searchByWords_clicked);
    connect(browseButton, &QPushButton::clicked, this, &PostSearch::on_browseButton_clicked);
}

// ----------------- UI SETUP -----------------

QPushButton* PostSearch::createStyledButton(const QString& text, const QString& style)
{
    QPushButton *button = new QPushButton(text);
    button->setStyleSheet(style);
    return button;
}

void PostSearch::setupUI()
{
    // Root Layout (0 margins, contains ScrollArea)
    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    // Scroll Area
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: transparent; border: none; }");

    // Scroll Content Widget
    QWidget *scrollAreaWidgetContents = new QWidget();
    scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
    scrollAreaWidgetContents->setStyleSheet("#scrollAreaWidgetContents { background-color: transparent; }");
    
    // Main Layout inside Scroll Area
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollAreaWidgetContents);
    mainLayout->setContentsMargins(24, 24, 24, 24);
    mainLayout->setSpacing(16);

    // 1. Navigation: Back Button (Directly in mainLayout)
    QPushButton *backBtn = new QPushButton("← Back to Operations");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setObjectName("backButton");
    backBtn->setStyleSheet(QString(
        "QPushButton#backButton {"
        "  background-color: transparent;"
        "  color: %1;"
        "  font-size: 14px;"
        "  border: none;"
        "  text-align: left;"
        "  padding: 8px;"
        "}"
        "QPushButton#backButton:hover { background-color: rgba(255, 109, 31, 0.1); border-radius: 6px; text-decoration: none; }"
    ).arg(ORANGE_THEME));
    
    connect(backBtn, &QPushButton::clicked, this, &PostSearch::on_backButton_clicked);
    mainLayout->addWidget(backBtn);

    // 2. Header Widget: Contained, rounded header card
    QWidget *headerWidget = new QWidget();
    headerWidget->setObjectName("headerWidget");
    headerWidget->setStyleSheet(QString(
        "#headerWidget {"
        "  background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ff6d1f, stop:1 #ef6c00);"
        "  border-radius: 12px;"
        "}"
    ));

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(24, 24, 24, 24);
    headerLayout->setSpacing(16);

    // Icon
    QLabel *iconLabel = new QLabel("🔎");
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(60, 60);
    iconLabel->setStyleSheet(QString(
        "background-color: white;"
        "color: %1;"
        "border-radius: 12px;"
        "font-size: 24px;"
        "font-weight: bold;"
    ).arg(ORANGE_THEME));
    headerLayout->addWidget(iconLabel);

    // Title & Description
    QVBoxLayout *headerTextLayout = new QVBoxLayout();
    headerTextLayout->setSpacing(4);

    QLabel *titleLabel = new QLabel("Post Search");
    titleLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold; background-color: transparent;");
    headerTextLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel("Finding posts related to topics or words");
    descLabel->setStyleSheet("color: rgba(255, 255, 255, 0.9); font-size: 13px; background-color: transparent;");
    headerTextLayout->addWidget(descLabel);

    headerLayout->addLayout(headerTextLayout);
    headerLayout->addStretch(); 

    mainLayout->addWidget(headerWidget);

    // 3. Main Content: White Card
    QWidget *contentWidget = new QWidget();
    contentWidget->setObjectName("contentWidget");
    contentWidget->setStyleSheet(
        "#contentWidget {"
        "  background-color: white;"
        "  border-radius: 12px;"
        "}"
    );

    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setContentsMargins(32, 32, 32, 32);
    contentLayout->setSpacing(20);

    // Input Label
    QLabel *inputLabel = new QLabel("Input XML File");
    inputLabel->setStyleSheet(QString("font-weight: 600; font-size: 14px; color: %1; background-color: transparent;").arg(TEXT_COLOR));
    contentLayout->addWidget(inputLabel);

    // Browse Button
    browseButton = new QPushButton("⬆  Browse XML File");
    browseButton->setCursor(Qt::PointingHandCursor);
    browseButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: #fff3e0;" 
        "  color: %1;"
        "  border: 1px solid #ffe0b2;"
        "  border-radius: 6px;"
        "  font-weight: 500;"
        "  padding: 12px 24px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #ffe0b2; }"
    ).arg(ORANGE_THEME));
    contentLayout->addWidget(browseButton);

    // OR Label
    QLabel *orLabel = new QLabel("OR");
    orLabel->setAlignment(Qt::AlignCenter);
    orLabel->setStyleSheet(QString("color: %1; font-size: 13px; background-color: transparent;").arg(ORANGE_THEME));
    contentLayout->addWidget(orLabel);

    // Text Area
    xmlTextEdit = new QTextEdit();
    xmlTextEdit->setPlaceholderText("Paste your XML content here...");
    xmlTextEdit->setMinimumHeight(200);
    xmlTextEdit->setStyleSheet(QString(
        "QTextEdit {"
        "  background-color: #f9fafb;"
        "  border: 1px solid #e5e7eb;"
        "  border-radius: 8px;"
        "  padding: 12px;"
        "  color: #374151;"
        "  font-size: 13px;"
        "  font-family: 'Courier New', monospace;"
        "}"
        "QTextEdit:focus { border: 2px solid %1; background-color: white; }"
    ).arg(ORANGE_THEME));
    contentLayout->addWidget(xmlTextEdit);

    // 4. Buttons (Post Search by Topics / Words)
    QHBoxLayout *actionsLayout = new QHBoxLayout();
    actionsLayout->setSpacing(16);

    QString actionButtonStyle = QString(
        "QPushButton {"
        "  background-color: %1;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: 600;"
        "  font-size: 15px;"
        "  padding: 12px 24px;"
        "}"
        "QPushButton:hover { background-color: #e65100; }"
    ).arg(ORANGE_THEME);

    postsearchByTopics = new QPushButton("Post search by topics");
    postsearchByTopics->setCursor(Qt::PointingHandCursor);
    postsearchByTopics->setStyleSheet(actionButtonStyle);
    
    postsearchByWords = new QPushButton("Post search by words");
    postsearchByWords->setCursor(Qt::PointingHandCursor);
    postsearchByWords->setStyleSheet(actionButtonStyle);

    actionsLayout->addWidget(postsearchByTopics);
    actionsLayout->addWidget(postsearchByWords);

    contentLayout->addLayout(actionsLayout);

    mainLayout->addWidget(contentWidget);
    mainLayout->addStretch(); // Fill remaining space

    scrollArea->setWidget(scrollAreaWidgetContents);
    rootLayout->addWidget(scrollArea);
}

// ----------------- SLOTS -----------------

void PostSearch::on_searchByTopics_clicked()
{
    // 1. Get user input (Topics)
    QInputDialog dialog(this);

    // 2. Set the window title and label text (matching the original call)
    dialog.setWindowTitle("Search by Topics");
    dialog.setLabelText("Enter topics:");
    dialog.setTextEchoMode(QLineEdit::Normal);
    
    // 3. APPLY STYLING
    dialog.setStyleSheet(
        "QDialog { background-color: white; }"
        "QLabel { color: #333; font-weight: bold; font-size: 14px; }"
        "QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 5px; color: #333; }"
        "QPushButton { background-color: " ORANGE_THEME "; color: white; padding: 8px 15px; border-radius: 5px; border: none; font-weight: bold; }"
        "QPushButton:hover { background-color: #E65100; }"
    );

    // 4. Execute the dialog and retrieve the result
    bool ok;
    QString input;
    if (dialog.exec() == QDialog::Accepted) {
        ok = true;
        input = dialog.textValue();
    } else {
        ok = false;
    }
    
    // 5. Use the result just as before
    if (!ok || input.isEmpty()) { 
        return; 
    }
    // 2. Get XML Content from QTextEdit
    QString xmlContentQt = xmlTextEdit->toPlainText();

    if (xmlContentQt.isEmpty()) {
        QMessageBox::warning(this, "Input Missing", "Please load or paste XML content before searching.");
        return;
    }

    // Convert QString to std::string for the core C++ function
    std::string xmlContentStd = xmlContentQt.toStdString();
    std::string topicStd = input.toStdString();

    // --- Core Search Logic ---
    try {
        // 3. Call the external function, returning PostMatchTC vector
        std::vector<PostMatch> results = searchPostsByTopic(xmlContentStd, topicStd);

        // 4. Format the Results for Display (Manual QString building)
        QString resultText;
        if (results.empty()) {
            resultText = QString("No posts found containing the topic: '%1'.").arg(input);
        } else {
            resultText = QString("✅ Found %1 matching posts containing topic: '%2'\n\n--- RESULTS ---\n").arg(results.size()).arg(input);
            
            for (size_t i = 0; i < results.size(); ++i) {
                std::string postIdStd = results[i].id;
                
                QString postIdDisplay;
                if (postIdStd.empty()) {
                    postIdDisplay = "(unknown)";
                } else {
                    postIdDisplay = QString::fromStdString(postIdStd);
                }
                
                resultText += QString("UserID: %1\n").arg(postIdDisplay);
                
                QString postText = QString::fromStdString(results[i].text).simplified();
                resultText += QString("Text: %1...\n\n").arg(postText.left(100)); // Limits output
            }
        }

        // 5. Display the Results
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🔍 Search Results - Topics");
        msgBox.setText(resultText);
        msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        msgBox.setMinimumWidth(500);

        // Styling
         msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QLabel { color: #333; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: " ORANGE_THEME "; color: white; border-radius: 5px; padding: 8px 15px; border: none; font-weight: bold; }"
            "QPushButton:hover { background-color: #E65100; }"
        );
        msgBox.exec();
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "🚨 Search Error", QString("An internal error occurred: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "🚨 Search Error", "An unknown error occurred during search execution.");
    }
}



void PostSearch::on_searchByWords_clicked()
{
    // 1. Get user input (Word)
   QInputDialog dialog(this);

    dialog.setWindowTitle("Search by Words");
    dialog.setLabelText("Enter specific word:");
    dialog.setTextEchoMode(QLineEdit::Normal);
    
    // 3. APPLY STYLING
    dialog.setStyleSheet(
        "QDialog { background-color: white; }"
        "QLabel { color: #333; font-weight: bold; font-size: 14px; }"
        "QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 5px; color: #333; }"
        "QPushButton { background-color: " ORANGE_THEME "; color: white; padding: 8px 15px; border-radius: 5px; border: none; font-weight: bold; }"
        "QPushButton:hover { background-color: #E65100; }"
    );

    // 4. Execute the dialog and retrieve the result
    bool ok;
    QString input;
    if (dialog.exec() == QDialog::Accepted) {
        ok = true;
        input = dialog.textValue();
    } else {
        ok = false;
    }
    
    // 5. Use the result just as before
    if (!ok || input.isEmpty()) { 
        return; 
    }
    // 2. Get XML Content from QTextEdit
    QString xmlContentQt = xmlTextEdit->toPlainText();

    if (xmlContentQt.isEmpty())
    {
        QMessageBox::warning(this, "Input Missing", "Please load or paste XML content before searching.");
        return;
    }

    std::string xmlContentStd = xmlContentQt.toStdString();
    std::string wordStd = input.toStdString();

    // --- Core Search Logic ---
    try {
        std::vector<PostMatch> results = searchPostsByWord(xmlContentStd,wordStd);

        // 4. Format the Results
        QString resultText;
        if (results.empty()) {
            resultText = QString("No posts found containing the word: '%1'.").arg(input);
        } else {
            resultText = QString("✅ Found %1 matching posts containing word: '%2'\n\n--- RESULTS ---\n").arg(results.size()).arg(input);
            
            for (size_t i = 0; i < results.size(); ++i) {
                std::string postIdStd = results[i].id;
                
                QString postIdDisplay;
                if (postIdStd.empty()) {
                    postIdDisplay = "(unknown)";
                } else {
                    postIdDisplay = QString::fromStdString(postIdStd);
                }
                
                resultText += QString("UserID: %1\n").arg(postIdDisplay);
                
                QString postText = QString::fromStdString(results[i].text).simplified();
                resultText += QString("Text: %1...\n\n").arg(postText.left(100)); // Limits output
            }
        }

        // 5. Display the Results
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("🔍 Search Results - Words");
        msgBox.setText(resultText);
        msgBox.setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard); 
        msgBox.setMinimumWidth(500);


        msgBox.setStyleSheet(
            "QMessageBox { background-color: white; }"
            "QLabel { color: #333; font-size: 14px; padding: 10px; }"
            "QPushButton { background-color: " ORANGE_THEME "; color: white; border-radius: 5px; padding: 8px 15px; border: none; font-weight: bold; }"
            "QPushButton:hover { background-color: #E65100; }"
        );
        msgBox.exec();

    } catch (const std::exception& e) {
        QMessageBox::critical(this, "🚨 Search Error", QString("An internal error occurred: %1").arg(e.what()));
    } catch (...) {
        QMessageBox::critical(this, "🚨 Search Error", "An unknown error occurred during search execution.");
    }
}

void PostSearch::on_browseButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Open Document", "", "Text Files (*.txt);;XML Files (*.xml);;All Files (*.*)"
    );

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            xmlTextEdit->setText(in.readAll());
        }
    }
}

void PostSearch::on_backButton_clicked()
{
    emit backClicked();
}

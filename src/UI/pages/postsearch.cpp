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
// #include <QGraphicsDropShadowEffect> // Start using effects if needed, but styling is mostly QSS
// #include "../../core/SearchTopic.h" 
#include "../../core/SearchWord.h" 
#include "../../core/SearchTopic.h" 

// Theme colors
#define ORANGE_THEME "#FF6D1F"
#define LIGHT_ORANGE_BG "#FFF3E0" // Lighter orange for browse button background
#define PAGE_BG "#F4F7FE" // Light gray/blue-ish background similar to reference
#define TEXT_COLOR "#2B3674" // Dark blue/gray text color often used with this style

PostSearch::PostSearch(QWidget *parent)
    : QMainWindow(parent)
{
    setMinimumSize(900, 750);
    setWindowTitle("Post Search");

    // Global styling
    setStyleSheet(QString(
        "QMainWindow { background-color: %1; }"
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
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    // Margins for the main page
    mainLayout->setContentsMargins(30, 20, 30, 30);
    mainLayout->setSpacing(20);

    // 1. Navigation: Back Link (Top-Left)
    // Move "Back" button out of the main container. It should be a simple text link.
    QPushButton *backBtn = new QPushButton("← Back to Operations");
    backBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: transparent;"
        "  color: %1;" // Use theme color or a dark text color
        "  font-size: 14px;"
        "  border: none;"
        "  text-align: left;"
        "  font-weight: 500;"
        "}"
        "QPushButton:hover { text-decoration: underline; }"
    ).arg(TEXT_COLOR));
    backBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    connect(backBtn, &QPushButton::clicked, this, &PostSearch::on_backButton_clicked);
    mainLayout->addWidget(backBtn);

    // 2. Header Structure: Contained, rounded header card
    QWidget *headerWidget = new QWidget();
    headerWidget->setObjectName("headerWidget");
    headerWidget->setStyleSheet(QString(
        "#headerWidget {"
        "  background-color: %1;" // Orange Theme
        "  border-radius: 20px;"
        "}"
    ).arg(ORANGE_THEME));

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 25, 25, 25);
    headerLayout->setSpacing(20);

    // Icon (Optional - Using a simple text icon or placeholder for now)
    QLabel *iconLabel = new QLabel("🔎"); // Using text representation of code/xml icon
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedSize(60, 60);
    iconLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 0.2);" // Semi-transparent white
        "color: white;"
        "border-radius: 12px;"
        "font-size: 24px;"
        "font-weight: bold;"
    );
    headerLayout->addWidget(iconLabel);

    // Title & Description Container
    QVBoxLayout *headerTextLayout = new QVBoxLayout();
    headerTextLayout->setSpacing(5);

    QLabel *titleLabel = new QLabel("Post Search");
    titleLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    headerTextLayout->addWidget(titleLabel);

    QLabel *descLabel = new QLabel("Finding posts related to topics or words");
    descLabel->setStyleSheet("color: rgba(255, 255, 255, 0.9); font-size: 14px;");
    headerTextLayout->addWidget(descLabel);

    headerLayout->addLayout(headerTextLayout);
    headerLayout->addStretch(); // Push content to left

    mainLayout->addWidget(headerWidget);

    // 3. Main Content: White Card
    QWidget *contentCard = new QWidget();
    contentCard->setObjectName("contentCard");
    contentCard->setStyleSheet(
        "#contentCard {"
        "  background-color: white;"
        "  border-radius: 20px;"
        "}"
    );

    QVBoxLayout *cardLayout = new QVBoxLayout(contentCard);
    cardLayout->setContentsMargins(30, 30, 30, 30);
    cardLayout->setSpacing(20);

    // Input Label
    QLabel *inputLabel = new QLabel("Input XML File");
    inputLabel->setStyleSheet(QString("font-weight: bold; font-size: 16px; color: %1;").arg(TEXT_COLOR));
    cardLayout->addWidget(inputLabel);

    // Browse Button
    browseButton = new QPushButton("  Browse XML File"); // Added spaces for loose centering if needed, or rely on layout
    browseButton->setCursor(Qt::PointingHandCursor);
    // Use an icon if possible, but text is fine. Reference has an upload icon.
    // mimicking reference style: light blue bg, blue text. Here: light orange bg, orange text.
    browseButton->setStyleSheet(QString(
        "QPushButton {"
        "  background-color: %1;" // Light Orange
        "  color: %2;" // Orange Theme
        "  border: none;"
        "  border-radius: 10px;"
        "  font-weight: bold;"
        "  padding: 15px;"
        "  font-size: 14px;"
        "}"
        "QPushButton:hover { background-color: #FFE0B2; }"
    ).arg(LIGHT_ORANGE_BG).arg(ORANGE_THEME));
    cardLayout->addWidget(browseButton);

    // Text Area
    xmlTextEdit = new QTextEdit();
    xmlTextEdit->setPlaceholderText("Paste your XML content here...");
    xmlTextEdit->setMinimumHeight(200);
    xmlTextEdit->setStyleSheet(
        "QTextEdit {"
        "  background-color: #FAFAFA;" // Very light gray
        "  border: 1px solid #E0E0E0;" // 1px border
        "  border-radius: 10px;"
        "  padding: 15px;"
        "  color: #333;"
        "  font-size: 14px;"
        "  font-family: Consolas, monospace;"
        "}"
    );
    cardLayout->addWidget(xmlTextEdit);

    cardLayout->addSpacing(10);

    // 4. Buttons (Post Search by Topics / Words)
    // Style: Match size and shape of primary button in reference.
    QHBoxLayout *actionsLayout = new QHBoxLayout();
    actionsLayout->setSpacing(15);

    QString actionButtonStyle = QString(
        "QPushButton {"
        "  background-color: %1;" // Orange Theme
        "  color: white;"
        "  border: none;"
        "  border-radius: 12px;" // Matching card radius proportion or reference button
        "  font-weight: bold;"
        "  font-size: 15px;"
        "  padding: 15px;"
        "}"
        "QPushButton:hover { background-color: #E65100; }" // Darker orange
        "QPushButton:pressed { background-color: #BF360C; }"
    ).arg(ORANGE_THEME);

    postsearchByTopics = new QPushButton("Post search by topics");
    postsearchByTopics->setCursor(Qt::PointingHandCursor);
    postsearchByTopics->setStyleSheet(actionButtonStyle);
    
    postsearchByWords = new QPushButton("Post search by words");
    postsearchByWords->setCursor(Qt::PointingHandCursor);
    postsearchByWords->setStyleSheet(actionButtonStyle);

    actionsLayout->addWidget(postsearchByTopics);
    actionsLayout->addWidget(postsearchByWords);

    cardLayout->addLayout(actionsLayout);

    mainLayout->addWidget(contentCard);
    mainLayout->addStretch(); // Fill remaining space
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
        // 3. Call the external function, returning PostMatchTopic vector
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

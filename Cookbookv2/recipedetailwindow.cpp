#include "RecipeDetailWindow.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

RecipeDetailWindow::RecipeDetailWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Recipe Details");
    resize(600, 500);

    titleLabel = new QLabel("Recipe", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: #92400E;");

    detailsText = new QTextEdit(this);
    detailsText->setReadOnly(true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(titleLabel);
    layout->addWidget(detailsText);

    setLayout(layout);

    setStyleSheet(R"(
        QWidget {
            background-color: #FAF6F2;
            color: #2F2F2F;
            font-family: "Helvetica Neue", Arial;
            font-size: 16px;
        }

        QTextEdit {
            background-color: #FFFFFF;
            border: 2px solid #D97706;
            border-radius: 12px;
            padding: 12px;
            font-size: 17px;
        }

        QLabel {
            font-size: 20px;
            font-weight: bold;
            color: #92400E;
        }
    )");
}

void RecipeDetailWindow::setRecipeData(const Recipe& recipe, const QStringList& ingredients, const QStringList& steps)
{
    titleLabel->setText(recipe.title);

    QString details;
    details += "Category: " + recipe.category + "\n";
    details += "Servings: " + QString::number(recipe.servings) + "\n";
    details += "Prep Time: " + QString::number(recipe.prepMinutes) + " min\n";
    details += "Cook Time: " + QString::number(recipe.cookMinutes) + " min\n";
    details += "Total Time: " + QString::number(recipe.totalMinutes) + " min\n\n";

    details += "Ingredients:\n";
    if (ingredients.isEmpty())
    {
        details += "No ingredients yet.\n";
    }
    else
    {
        for (const QString& ingredient : ingredients)
            details += "- " + ingredient + "\n";
    }

    details += "\nInstruction Steps:\n";
    if (steps.isEmpty())
    {
        details += "No steps yet.\n";
    }
    else
    {
        int stepNumber = 1;
        for (const QString& step : steps)
        {
            details += QString::number(stepNumber) + ". " + step + "\n";
            stepNumber++;
        }
    }

    details += "\nDescription:\n";
    details += recipe.description.isEmpty() ? "No description yet." : recipe.description;

    detailsText->setPlainText(details);
}

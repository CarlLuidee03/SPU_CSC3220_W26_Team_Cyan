#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QInputDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include "recipedetailwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setStyleSheet(R"(

QWidget {
    background-color: #FAF6F2;
    color: #2F2F2F;
    font-family: "Helvetica Neue", Arial;
    font-size: 16px;
}

/* Recipe list */
QListWidget {
    background-color: #FFFFFF;
    border: 2px solid #D97706;
    border-radius: 12px;
    padding: 10px;
    font-size: 18px;
}

QListWidget::item {
    padding: 8px;
}

QListWidget::item:selected {
    background-color: #FDBA74;
    color: #1F2937;
    border-radius: 6px;
}

/* Recipe details box */
QTextEdit {
    background-color: #FFFFFF;
    border: 2px solid #D97706;
    border-radius: 12px;
    padding: 12px;
    font-size: 17px;
}

/* Buttons */
QPushButton {
    background-color: #F59E0B;
    color: white;
    border-radius: 12px;
    padding: 12px;
    font-size: 18px;
    font-weight: bold;
}

QPushButton:hover {
    background-color: #D97706;
}

QPushButton:pressed {
    background-color: #B45309;
}

/* Section headers */
QLabel {
    font-size: 20px;
    font-weight: bold;
    color: #92400E;
}

)");

    connect(ui->addRecipeButton, &QPushButton::clicked, this, &MainWindow::onAddRecipeClicked);
    connect(ui->recipeList, &QListWidget::itemSelectionChanged, this, &MainWindow::showRecipeDetails);
    connect(ui->deleteRecipeButton, &QPushButton::clicked, this, &MainWindow::onDeleteRecipeClicked);
    connect(ui->editRecipeButton, &QPushButton::clicked, this, &MainWindow::onEditRecipeClicked);

    loadRecipes();
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::loadRecipes()
{
    ui->recipeList->clear();
    ui->recipeDetails->clear();

    QVector<Recipe> recipes = m_repository.getAllRecipes();

    qDebug() << "Loaded recipes:" << recipes.size();

    for (const Recipe& recipe : recipes)
    {
        QListWidgetItem *item = new QListWidgetItem(recipe.title);
        item->setData(Qt::UserRole, recipe.id);
        ui->recipeList->addItem(item);
    }
}
void MainWindow::onAddRecipeClicked()
{
    bool ok = false;

    QString title = QInputDialog::getText(
        this,
        "Add Recipe",
        "Recipe title:",
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || title.trimmed().isEmpty())
        return;

    QString category = QInputDialog::getText(
        this,
        "Add Recipe",
        "Category:",
        QLineEdit::Normal,
        "Uncategorized",
        &ok
        );

    if (!ok)
        return;

    int servings = QInputDialog::getInt(
        this,
        "Add Recipe",
        "Servings:",
        1,
        1,
        100,
        1,
        &ok
        );

    if (!ok)
        return;

    int prepMinutes = QInputDialog::getInt(
        this,
        "Add Recipe",
        "Prep time (minutes):",
        0,
        0,
        1000,
        1,
        &ok
        );

    if (!ok)
        return;

    int cookMinutes = QInputDialog::getInt(
        this,
        "Add Recipe",
        "Cook time (minutes):",
        0,
        0,
        1000,
        1,
        &ok
        );

    if (!ok)
        return;

    QString description = QInputDialog::getMultiLineText(
        this,
        "Add Recipe",
        "Description:",
        "",
        &ok
        );

    if (!ok)
        return;
    QString ingredientsText = QInputDialog::getMultiLineText(
        this,
        "Add Recipe",
        "Ingredients (one per line):",
        "",
        &ok
        );

    if (!ok)
        return;
    QString stepsText = QInputDialog::getMultiLineText(
        this,
        "Add Recipe",
        "Instruction steps (one per line):",
        "",
        &ok
        );

    if (!ok)
        return;

    Recipe recipe;
    recipe.title = title.trimmed();
    recipe.category = category.trimmed().isEmpty() ? "Uncategorized" : category.trimmed();
    recipe.servings = servings;
    recipe.prepMinutes = prepMinutes;
    recipe.cookMinutes = cookMinutes;
    recipe.description = description.trimmed();


    if (m_repository.createRecipe(recipe))
    {
        QStringList ingredients = ingredientsText.split('\n', Qt::SkipEmptyParts);
        m_repository.addIngredients(recipe.id, ingredients);

        QStringList steps = stepsText.split('\n', Qt::SkipEmptyParts);
        m_repository.addInstructions(recipe.id, steps);

        loadRecipes();
    }

}
void MainWindow::showRecipeDetails()
{
    QListWidgetItem *item = ui->recipeList->currentItem();

    if (!item)
    {
        ui->recipeDetails->clear();
        return;
    }

    int recipeId = item->data(Qt::UserRole).toInt();

    Recipe recipe;
    if (!m_repository.getRecipeById(recipeId, recipe))
    {
        ui->recipeDetails->setPlainText("Could not load recipe details.");
        return;
    }

    QStringList ingredients = m_repository.getIngredients(recipe.id);
    QStringList steps = m_repository.getInstructions(recipe.id);

    RecipeDetailWindow *detailWindow = new RecipeDetailWindow(this);
    detailWindow->setRecipeData(recipe, ingredients, steps);
    detailWindow->show();


    QString details;
    details += "Title: " + recipe.title + "\n";
    details += "Category: " + recipe.category + "\n";
    details += "Servings: " + QString::number(recipe.servings) + "\n";
    details += "Prep Time: " + QString::number(recipe.prepMinutes) + "\n";
    details += "Cook Time: " + QString::number(recipe.cookMinutes) + "\n";
    details += "Total Time: " + QString::number(recipe.totalMinutes) + "\n";


    details += "\nIngredients:\n";
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

    ui->recipeDetails->setPlainText(details);
}
void MainWindow::onDeleteRecipeClicked()
{
    QListWidgetItem *item = ui->recipeList->currentItem();

    if (!item)
        return;

    int recipeId = item->data(Qt::UserRole).toInt();

    auto reply = QMessageBox::question(
        this,
        "Delete Recipe",
        "Are you sure you want to delete this recipe?"
        );

    if (reply != QMessageBox::Yes)
        return;

    if (m_repository.deleteRecipe(recipeId))
    {
        loadRecipes();
    }
}
void MainWindow::onEditRecipeClicked()
{
    QListWidgetItem *item = ui->recipeList->currentItem();

    if (!item)
        return;

    int recipeId = item->data(Qt::UserRole).toInt();

    Recipe recipe;
    if (!m_repository.getRecipeById(recipeId, recipe))
        return;

    bool ok = false;

    QString title = QInputDialog::getText(
        this,
        "Edit Recipe",
        "Recipe title:",
        QLineEdit::Normal,
        recipe.title,
        &ok
        );
    if (!ok || title.trimmed().isEmpty())
        return;

    QString category = QInputDialog::getText(
        this,
        "Edit Recipe",
        "Category:",
        QLineEdit::Normal,
        recipe.category,
        &ok
        );
    if (!ok)
        return;

    int servings = QInputDialog::getInt(
        this,
        "Edit Recipe",
        "Servings:",
        recipe.servings,
        1,
        100,
        1,
        &ok
        );
    if (!ok)
        return;

    int prepMinutes = QInputDialog::getInt(
        this,
        "Edit Recipe",
        "Prep time (minutes):",
        recipe.prepMinutes,
        0,
        1000,
        1,
        &ok
        );
    if (!ok)
        return;

    int cookMinutes = QInputDialog::getInt(
        this,
        "Edit Recipe",
        "Cook time (minutes):",
        recipe.cookMinutes,
        0,
        1000,
        1,
        &ok
        );
    if (!ok)
        return;

    QString description = QInputDialog::getMultiLineText(
        this,
        "Edit Recipe",
        "Description:",
        recipe.description,
        &ok
        );
    if (!ok)
        return;
    QString ingredientsText = QInputDialog::getMultiLineText(
        this,
        "Edit Recipe",
        "Ingredients (one per line):",
        "",
        &ok
        );

    if (!ok)
        return;

    QString stepsText = QInputDialog::getMultiLineText(
        this,
        "Edit Recipe",
        "Instruction steps (one per line):",
        "",
        &ok
        );

    if (!ok)
        return;

    recipe.title = title.trimmed();
    recipe.category = category.trimmed().isEmpty() ? "Uncategorized" : category.trimmed();
    recipe.servings = servings;
    recipe.prepMinutes = prepMinutes;
    recipe.cookMinutes = cookMinutes;
    recipe.description = description.trimmed();

    if (m_repository.updateRecipe(recipe))
    {
        m_repository.deleteIngredients(recipe.id);
        m_repository.deleteInstructions(recipe.id);

        QStringList ingredients = ingredientsText.split('\n', Qt::SkipEmptyParts);
        m_repository.addIngredients(recipe.id, ingredients);

        QStringList steps = stepsText.split('\n', Qt::SkipEmptyParts);
        m_repository.addInstructions(recipe.id, steps);

        loadRecipes();
    }
}

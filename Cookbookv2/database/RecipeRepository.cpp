#include "RecipeRepository.h"
#include "DatabaseManager.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

bool RecipeRepository::createRecipe(Recipe& recipe)
{
    recipe.computeTotalTime();

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        INSERT INTO recipes(title, category, servings, prep_minutes, cook_minutes, total_minutes, description)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

    query.addBindValue(recipe.title);
    query.addBindValue(recipe.category);
    query.addBindValue(recipe.servings);
    query.addBindValue(recipe.prepMinutes);
    query.addBindValue(recipe.cookMinutes);
    query.addBindValue(recipe.totalMinutes);
    query.addBindValue(recipe.description);

    if (!query.exec())
        return false;

    recipe.id = query.lastInsertId().toInt();
    return true;

}

QVector<Recipe> RecipeRepository::getAllRecipes() const
{
    QVector<Recipe> recipes;

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        SELECT id, title, category, servings, prep_minutes, cook_minutes, total_minutes, description
        FROM recipes
        ORDER BY title
    )");

    if (!query.exec())
        return recipes;

    while (query.next())
    {
        Recipe recipe;
        recipe.id = query.value(0).toInt();
        recipe.title = query.value(1).toString();
        recipe.category = query.value(2).toString();
        recipe.servings = query.value(3).toInt();
        recipe.prepMinutes = query.value(4).toInt();
        recipe.cookMinutes = query.value(5).toInt();
        recipe.totalMinutes = query.value(6).toInt();
        recipe.description = query.value(7).toString();

        recipes.push_back(recipe);
    }

    return recipes;
}
bool RecipeRepository::getRecipeById(int recipeId, Recipe& recipe) const
{
    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        SELECT id, title, category, servings, prep_minutes, cook_minutes, total_minutes, description
        FROM recipes
        WHERE id = ?
    )");

    query.addBindValue(recipeId);

    if (!query.exec())
        return false;

    if (!query.next())
        return false;

    recipe.id = query.value(0).toInt();
    recipe.title = query.value(1).toString();
    recipe.category = query.value(2).toString();
    recipe.servings = query.value(3).toInt();
    recipe.prepMinutes = query.value(4).toInt();
    recipe.cookMinutes = query.value(5).toInt();
    recipe.totalMinutes = query.value(6).toInt();
    recipe.description = query.value(7).toString();

    return true;
}
bool RecipeRepository::deleteRecipe(int recipeId)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare("DELETE FROM recipes WHERE id = ?");
    query.addBindValue(recipeId);

    return query.exec();
}
bool RecipeRepository::updateRecipe(const Recipe& recipe)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        UPDATE recipes
        SET title = ?, category = ?, servings = ?, prep_minutes = ?, cook_minutes = ?, total_minutes = ?, description = ?
        WHERE id = ?
    )");

    query.addBindValue(recipe.title);
    query.addBindValue(recipe.category);
    query.addBindValue(recipe.servings);
    query.addBindValue(recipe.prepMinutes);
    query.addBindValue(recipe.cookMinutes);
    query.addBindValue(recipe.prepMinutes + recipe.cookMinutes);
    query.addBindValue(recipe.description);
    query.addBindValue(recipe.id);

    return query.exec();
}
bool RecipeRepository::addIngredients(int recipeId, const QStringList& ingredients)
{
    QSqlDatabase db = DatabaseManager::instance().database();

    for (const QString& ingredientName : ingredients)
    {
        QString trimmed = ingredientName.trimmed();
        if (trimmed.isEmpty())
            continue;

        QSqlQuery query(db);
        query.prepare(R"(
            INSERT INTO ingredients (recipe_id, name, quantity, unit)
            VALUES (?, ?, 0, '')
        )");
        query.addBindValue(recipeId);
        query.addBindValue(trimmed);

        if (!query.exec())
            return false;
    }

    return true;
}

QStringList RecipeRepository::getIngredients(int recipeId) const
{
    QStringList ingredients;

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        SELECT name
        FROM ingredients
        WHERE recipe_id = ?
        ORDER BY id
    )");
    query.addBindValue(recipeId);

    if (!query.exec())
        return ingredients;

    while (query.next())
    {
        ingredients << query.value(0).toString();
    }

    return ingredients;
}
bool RecipeRepository::addInstructions(int recipeId, const QStringList& steps)
{
    QSqlDatabase db = DatabaseManager::instance().database();

    int stepNumber = 1;

    for (const QString& stepText : steps)
    {
        QString trimmed = stepText.trimmed();
        if (trimmed.isEmpty())
            continue;

        QSqlQuery query(db);
        query.prepare(R"(
            INSERT INTO instructions (recipe_id, step_number, instruction)
            VALUES (?, ?, ?)
        )");
        query.addBindValue(recipeId);
        query.addBindValue(stepNumber);
        query.addBindValue(trimmed);

        if (!query.exec())
            return false;

        stepNumber++;
    }

    return true;
}

QStringList RecipeRepository::getInstructions(int recipeId) const
{
    QStringList steps;

    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare(R"(
        SELECT instruction
        FROM instructions
        WHERE recipe_id = ?
        ORDER BY step_number
    )");
    query.addBindValue(recipeId);

    if (!query.exec())
        return steps;

    while (query.next())
    {
        steps << query.value(0).toString();
    }

    return steps;
}
bool RecipeRepository::deleteIngredients(int recipeId)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare("DELETE FROM ingredients WHERE recipe_id = ?");
    query.addBindValue(recipeId);

    return query.exec();
}

bool RecipeRepository::deleteInstructions(int recipeId)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    QSqlQuery query(db);

    query.prepare("DELETE FROM instructions WHERE recipe_id = ?");
    query.addBindValue(recipeId);

    return query.exec();
}

#pragma once

#include "../models/Recipe.h"
#include <QVector>

class RecipeRepository
{
public:
    bool createRecipe(Recipe& recipe);
    QVector<Recipe> getAllRecipes() const;
    bool getRecipeById(int recipeId, Recipe& recipe) const;
    bool deleteRecipe(int recipeId);
    bool updateRecipe(const Recipe& recipe);
    bool addIngredients(int recipeId, const QStringList& ingredients);
    QStringList getIngredients(int recipeId) const;
    bool addInstructions(int recipeId, const QStringList& steps);
    QStringList getInstructions(int recipeId) const;
    bool deleteIngredients(int recipeId);
    bool deleteInstructions(int recipeId);
};

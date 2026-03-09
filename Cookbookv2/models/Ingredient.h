#pragma once

#include <QString>

struct Ingredient
{
    int id = -1;
    int recipeId = -1;

    QString name;
    double quantity = 0.0;
    QString unit;
};


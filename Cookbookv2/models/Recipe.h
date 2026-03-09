#pragma once

#include <QString>
#include <QVector>

#include <QString>
#include <QVector>
#include "Ingredient.h"
#include "InstructionStep.h"

struct Recipe
{
    int id = -1;

    QString title;
    QString category;

    int servings = 1;

    int prepMinutes = 0;
    int cookMinutes = 0;
    int totalMinutes = 0;

    QString description;

    QVector<Ingredient> ingredients;
    QVector<InstructionStep> instructions;

    void computeTotalTime()
    {
        totalMinutes = prepMinutes + cookMinutes;
    }
};

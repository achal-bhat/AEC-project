#include <iostream>
#include <string>
#include <limits> // Required for numeric_limits
#include <iomanip> // Required for setprecision

using namespace std;

int main() {
    // Increased safety: Using constants
    const int MAX_FOODS = 200;
    
    string food[MAX_FOODS];
    double protein[MAX_FOODS], carbs[MAX_FOODS], fats[MAX_FOODS], calories[MAX_FOODS];
    int count = 0;

    double totalCalories = 0.0;
    double totalProtein = 0.0;
    double totalCarbs = 0.0;
    double totalFats = 0.0;

    int weight = 0;
    int maintenance = 0;
    int goalCalories = 0;
    int goalType = 0; // 1=maintain, 2=bulk, 3=cut

    int choice;

    while (true) {
        cout << "\n====== SIMPLE CALORIE TRACKER ======\n";
        cout << "1. Set Weight & Goal\n";
        cout << "2. Add Food\n";
        cout << "3. View Food History\n";
        cout << "4. View Totals & Progress\n";
        cout << "5. Delete Last Food\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        
        // INPUT VALIDATION: Prevents infinite loop if user types a letter
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            continue;
        }

        // SET WEIGHT AND GOAL
        if (choice == 1) {
            cout << "Enter weight (kg): ";
            while(!(cin >> weight)) { // Validation loop
                cout << "Invalid input. Enter weight as a number: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }

            maintenance = weight * 33; // Rough estimate calculation

            cout << "\nMaintenance Calories: " << maintenance << " kcal\n";
            cout << "Choose goal:\n";
            cout << "1. Maintenance\n";
            cout << "2. Bulking (+300)\n";
            cout << "3. Cutting (-500)\n";
            cout << "Enter choice: ";
            cin >> goalType;

            if (goalType == 1)
                goalCalories = maintenance;
            else if (goalType == 2)
                goalCalories = maintenance + 300;
            else if (goalType == 3)
                goalCalories = maintenance - 500;
            else {
                cout << "Invalid goal choice. Defaulting to Maintenance.\n";
                goalCalories = maintenance;
            }

            cout << "Goal calories set to " << goalCalories << " kcal\n";
        }

        // ADD FOOD
        else if (choice == 2) {
            // BOUNDS CHECK: Prevent crashing if array is full
            if (count >= MAX_FOODS) {
                cout << "Memory full! Delete some items before adding more.\n";
                continue;
            }

            // BUFFER FIX: Clear newline from buffer before using getline
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

            cout << "Food name: ";
            getline(cin, food[count]);

            cout << "Protein (g): ";
            cin >> protein[count];
            cout << "Carbs (g): ";
            cin >> carbs[count];
            cout << "Fats (g): ";
            cin >> fats[count];

            calories[count] = (protein[count] * 4) + (carbs[count] * 4) + (fats[count] * 9);

            totalProtein += protein[count];
            totalCarbs += carbs[count];
            totalFats += fats[count];
            totalCalories += calories[count];

            cout << "\n✔ Added: " << food[count] << "\n";
            cout << "Calories: " << calories[count] << " kcal\n";

            if (goalCalories > 0) {
                int diff = totalCalories - goalCalories;
                if (diff > 0)
                    cout << "⚠ Above goal by " << diff << " kcal\n";
                else
                    cout << "Remaining: " << -diff << " kcal\n";
            }

            count++;
        }

        // FOOD HISTORY
        else if (choice == 3) {
            if (count == 0)
                cout << "No food history yet.\n";
            else {
                cout << "\n--- FOOD HISTORY ---\n";
                for (int i = 0; i < count; i++) {
                    cout << i + 1 << ". " << food[i]
                         << " | P:" << protein[i]
                         << " C:" << carbs[i]
                         << " F:" << fats[i]
                         << " | " << calories[i]
                         << " kcal\n";
                }
            }
        }

        // TOTALS & PROGRESS
        else if (choice == 4) {
            if (totalCalories == 0) {
                cout << "No food added yet.\n";
            } else {
                double proteinCal = totalProtein * 4.0;
                double carbsCal = totalCarbs * 4.0;
                double fatsCal = totalFats * 9.0;

                cout << "\n--- TOTALS ---\n";
                cout << "Calories: " << totalCalories << " kcal\n";
                cout << "Protein : " << totalProtein << " g\n";
                cout << "Carbs   : " << totalCarbs << " g\n";
                cout << "Fats    : " << totalFats << " g\n";

                // Added setprecision for cleaner percentages
                cout << fixed << setprecision(1); 
                cout << "\n--- MACRO % ---\n";
                // Explicit casting to double to avoid integer division issues
                cout << "Protein: " << (proteinCal / totalCalories) * 100.0 << "%\n";
                cout << "Carbs  : " << (carbsCal / totalCalories) * 100.0 << "%\n";
                cout << "Fats   : " << (fatsCal / totalCalories) * 100.0 << "%\n";
                cout << defaultfloat; // Reset formatting

                if (goalCalories > 0) {
                    int diff = totalCalories - goalCalories;

                    cout << "\n--- PROGRESS ---\n";
                    if (goalType == 2) { // bulk
                        if (diff >= 0)
                            cout << "📈 On track for bulking.\n";
                        else
                            cout << "📉 Eat more for bulking.\n";
                    }
                    else if (goalType == 3) { // cut
                        if (diff <= 0)
                            cout << "📉 Good deficit for cutting.\n";
                        else
                            cout << "⚠ Too many calories for cutting.\n";
                    }
                    else { // maintenance
                        // Added a small buffer range for maintenance
                        if (diff > -50 && diff < 50) 
                            cout << "⚖ Perfect maintenance.\n";
                        else if (diff <= -50)
                            cout << "⚠ Under maintenance calories.\n";
                        else
                            cout << "⚠ Over maintenance calories.\n";
                    }
                } else {
                    cout << "\n(Set a goal in Menu Option 1 to see progress)\n";
                }
            }
        }

        // DELETE LAST FOOD
        else if (choice == 5) {
            if (count == 0)
                cout << "Nothing to delete.\n";
            else {
                count--; // Move index back
                totalProtein -= protein[count];
                totalCarbs -= carbs[count];
                totalFats -= fats[count];
                totalCalories -= calories[count];
                
                cout << "Last food (" << food[count] << ") removed.\n";
            }
        }

        // EXIT
        else if (choice == 6) {
            cout << "Exiting... Stay consistent 💪\n";
            break;
        }

        else {
            cout << "Invalid choice.\n";
        }
    }

    return 0;
}
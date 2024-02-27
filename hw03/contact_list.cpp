#include "contact_list.h"

#include <algorithm>
#include <numeric>
#include <sstream>

// TODO create implementation here!
using number_t = int64_t;

bool contact_list::add(storage &contacts, std::string_view name, number_t number)
{
    // Check if the name and number already exists in the contact list
    if (get_number_by_name(contacts, name) != -1 || get_name_by_number(contacts, number) != "")
    {
        return false;
    }
    // Add the new contact to the end of the vectors
    contacts.names.push_back(std::string(name));
    contacts.numbers.push_back(number);
    return true;
}

size_t contact_list::size(const storage &contacts)
{
    return contacts.names.size() == contacts.numbers.size() ? contacts.names.size() : 0;
}

number_t contact_list::get_number_by_name(storage &contacts, std::string_view name)
{
    // Find the index of the name in the vector of names
    auto i = std::find(contacts.names.begin(), contacts.names.end(), name);
    // If the name is not in the vector, return -1
    if (i == contacts.names.end())
    {
        return -1;
    }
    auto idx = std::distance(contacts.names.begin(), i);

    return contacts.numbers[idx];
}

std::string contact_list::to_string(const storage &contacts)
{
    std::stringstream ss;
    for (size_t i = 0; i < size(contacts); ++i)
    {
        ss << contacts.names[i] << " - " << contacts.numbers[i] << '\n';
    }
    return ss.str();
}

bool contact_list::remove(storage &contacts, std::string_view name)
{
    auto iName = std::find(contacts.names.begin(), contacts.names.end(), name);
    if (iName == contacts.names.end())
    {
        return false;
    }

    number_t number = get_number_by_name(contacts, name);

    auto iNumber = std::find(contacts.numbers.begin(), contacts.numbers.end(), number);
    if (iNumber == contacts.numbers.end())
    {
        return false;
    }

    contacts.names.erase(iName);
    contacts.numbers.erase(iNumber);
    return true;
}

void contact_list::sort(storage &contacts)
{
    size_t n = size(contacts);
    bool swapped;

    for (size_t i = 0; i < n; i++)
    {
        swapped = false;
        for (size_t j = 1; j < (n - i); j++)
            if (contacts.names[j - 1] > contacts.names[j])
            {
                auto tempName = contacts.names[j - 1];
                auto tempNumber = contacts.numbers[j - 1];
                contacts.names[j - 1] = contacts.names[j];
                contacts.numbers[j - 1] = contacts.numbers[j];
                contacts.names[j] = tempName;
                contacts.numbers[j] = tempNumber;
                swapped = true;
            }
        if (!swapped)
            break;
    }
}

std::string contact_list::get_name_by_number(storage &contacts, number_t number)
{
    // Find the index of the number in the vector of numbers
    auto i = std::find(contacts.numbers.begin(), contacts.numbers.end(), number);
    // If the name is not in the vector, return -1
    if (i == contacts.numbers.end())
    {
        return "";
    }
    auto idx = std::distance(contacts.numbers.begin(), i);

    return contacts.names[idx];
}

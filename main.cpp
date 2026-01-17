#pragma once

#include <iostream>
#include <initializer_list>
#include <functional>
#include <memory>
#include <limits>
#include <sstream>
#include <string>
#include "lazy_sequence.hpp"
#include "ordinal.hpp"
#include "generators.hpp"
#include "lab3_2ndsem/headers/array_sequence.hpp"


void clear_input()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int get_int_input(const std::string &prompt)
{
    int value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            clear_input();
            return value;
        }
        else
        {
            clear_input();
            std::cout << "Error: please enter an integer!" << std::endl;
        }
    }
}

double get_double_input(const std::string &prompt)
{
    double value;
    while (true)
    {
        std::cout << prompt;
        if (std::cin >> value)
        {
            clear_input();
            return value;
        }
        else
        {
            clear_input();
            std::cout << "Error: please enter a number!" << std::endl;
        }
    }
}

// Functions for working with ordinal
ordinal create_ordinal_from_string()
{
    std::cout << "Enter ordinal in format: 'coefficient1 exponent1 coefficient2 exponent2 ...'" << std::endl;
    std::cout << "Example: '1 0 2 1' means ω^1 * 2 + 1" << std::endl;
    std::cout << "For finite number enter just number (example: '5')" << std::endl;

    std::string input;
    std::getline(std::cin, input);

    if (input.empty())
    {
        return ordinal();
    }

    // Check if it's just a number
    bool is_simple_number = true;
    for (char c : input)
    {
        if (!isdigit(c) && c != ' ' && c != '-')
        {
            is_simple_number = false;
            break;
        }
    }

    if (is_simple_number)
    {
        std::stringstream ss(input);
        int num;
        if (ss >> num)
        {
            return ordinal(num);
        }
    }

    // Parse terms
    std::stringstream ss(input);
    int coeff, exp;
    array_sequence<term> terms;

    while (ss >> coeff >> exp)
    {
        terms.append_element(term(coeff, exp));
    }

    if (terms.get_length() == 0)
    {
        return ordinal();
    }

    array_sequence<term> cantor_form;
    for (const auto& t : terms)
    {
        cantor_form.append_element(t);
    }
    
    return ordinal(cantor_form);
}

void print_ordinal(const ordinal &ord)
{
    if (ord.is_finite())
    {
        std::cout << "Finite ordinal: " << ord.get_numerical_part() << std::endl;
    }
    else
    {
        std::cout << "Ordinal: ";
        auto form = ord.get_form();
        for (int i = 0; i < form.get_length(); i++)
        {
            const term &t = form.get(i);
            if (i > 0)
                std::cout << " + ";
            std::cout << "ω^" << t.get_exponent() << " * " << t.get_coefficient();
        }
        if (ord.get_numerical_part() > 0)
        {
            std::cout << " + " << ord.get_numerical_part();
        }
        std::cout << std::endl;
    }
}

void run_ordinal_menu()
{
    ordinal a, b;
    bool a_created = false, b_created = false;

    while (true)
    {
        std::cout << "\n=== Ordinal Operations Menu ===" << std::endl;
        std::cout << "1. Create ordinal A" << std::endl;
        std::cout << "2. Create ordinal B" << std::endl;
        std::cout << "3. Show ordinals" << std::endl;
        std::cout << "4. A + B" << std::endl;
        std::cout << "5. A - B" << std::endl;
        std::cout << "6. A * B" << std::endl;
        std::cout << "7. A + number" << std::endl;
        std::cout << "8. A * number" << std::endl;
        std::cout << "9. A += B" << std::endl;
        std::cout << "10. A++" << std::endl;
        std::cout << "11. ++A" << std::endl;
        std::cout << "12. Compare A and B" << std::endl;
        std::cout << "13. Check if A is finite" << std::endl;
        std::cout << "14. Return to main menu" << std::endl;
        std::cout << "0. Exit" << std::endl;

        int choice = get_int_input("Choose operation: ");

        try
        {
            switch (choice)
            {
            case 1:
                a = create_ordinal_from_string();
                a_created = true;
                std::cout << "Ordinal A created." << std::endl;
                break;

            case 2:
                b = create_ordinal_from_string();
                b_created = true;
                std::cout << "Ordinal B created." << std::endl;
                break;

            case 3:
                if (a_created)
                {
                    std::cout << "Ordinal A: ";
                    print_ordinal(a);
                }
                else
                {
                    std::cout << "Ordinal A not created." << std::endl;
                }
                if (b_created)
                {
                    std::cout << "Ordinal B: ";
                    print_ordinal(b);
                }
                else
                {
                    std::cout << "Ordinal B not created." << std::endl;
                }
                break;

            case 4:
                if (a_created && b_created)
                {
                    ordinal result = a + b;
                    std::cout << "Result A + B: ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create both ordinals first!" << std::endl;
                }
                break;

            case 5:
                if (a_created && b_created)
                {
                    ordinal result = a - b;
                    std::cout << "Result A - B: ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create both ordinals first!" << std::endl;
                }
                break;

            case 6:
                if (a_created && b_created)
                {
                    ordinal result = a * b;
                    std::cout << "Result A * B: ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create both ordinals first!" << std::endl;
                }
                break;

            case 7:
                if (a_created)
                {
                    int num = get_int_input("Enter number: ");
                    ordinal result = a + num;
                    std::cout << "Result A + " << num << ": ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create ordinal A first!" << std::endl;
                }
                break;

            case 8:
                if (a_created)
                {
                    int num = get_int_input("Enter number: ");
                    ordinal result = a * num;
                    std::cout << "Result A * " << num << ": ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create ordinal A first!" << std::endl;
                }
                break;

            case 9:
                if (a_created && b_created)
                {
                    a += b;
                    std::cout << "Executed A += B" << std::endl;
                    std::cout << "New A: ";
                    print_ordinal(a);
                }
                else
                {
                    std::cout << "Please create both ordinals first!" << std::endl;
                }
                break;

            case 10:
                if (a_created)
                {
                    ordinal result = a++;
                    std::cout << "Executed A++" << std::endl;
                    std::cout << "Result (old value): ";
                    print_ordinal(result);
                    std::cout << "New A: ";
                    print_ordinal(a);
                }
                else
                {
                    std::cout << "Please create ordinal A first!" << std::endl;
                }
                break;

            case 11:
                if (a_created)
                {
                    ordinal result = ++a;
                    std::cout << "Executed ++A" << std::endl;
                    std::cout << "New A: ";
                    print_ordinal(result);
                }
                else
                {
                    std::cout << "Please create ordinal A first!" << std::endl;
                }
                break;

            case 12:
                if (a_created && b_created)
                {
                    std::cout << "A == B: " << (a == b ? "yes" : "no") << std::endl;
                    std::cout << "A != B: " << (a != b ? "yes" : "no") << std::endl;
                    std::cout << "A < B: " << (a < b ? "yes" : "no") << std::endl;
                    std::cout << "A > B: " << (a > b ? "yes" : "no") << std::endl;
                    std::cout << "A <= B: " << (a <= b ? "yes" : "no") << std::endl;
                    std::cout << "A >= B: " << (a >= b ? "yes" : "no") << std::endl;
                }
                else
                {
                    std::cout << "Please create both ordinals first!" << std::endl;
                }
                break;

            case 13:
                if (a_created)
                {
                    std::cout << "Ordinal A is " << (a.is_finite() ? "finite" : "infinite") << std::endl;
                }
                else
                {
                    std::cout << "Please create ordinal A first!" << std::endl;
                }
                break;

            case 14:
                return;

            case 0:
                exit(0);

            default:
                std::cout << "Invalid choice!" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

// Functions for working with lazy_sequence
template <typename T>
lazy_sequence<T> *create_lazy_sequence()
{
    std::cout << "\nChoose generator type:" << std::endl;
    std::cout << "1. Sequential numbers (0, 1, 2, ...)" << std::endl;
    std::cout << "2. Even numbers (0, 2, 4, ...)" << std::endl;
    std::cout << "3. Odd numbers (1, 3, 5, ...)" << std::endl;
    std::cout << "4. Fibonacci numbers" << std::endl;
    std::cout << "5. Powers of two (1, 2, 4, 8, ...)" << std::endl;

    int gen_choice = get_int_input("Your choice: ");

    generator<T> *gen = nullptr;

    switch (gen_choice)
    {
    case 1:
        gen = new increment_generator<T>(0, 1);
        break;
    case 2:
        gen = new increment_generator<T>(0, 2);
        break;
    case 3:
        gen = new increment_generator<T>(1, 2);
        break;
    case 4:
        gen = new fibonacci_generator<T>(0, 1);
        break;
    case 5:
        gen = new power_generator<T>(2);
        break;
    default:
        std::cout << "Invalid choice, using default generator." << std::endl;
        gen = new increment_generator<T>(0, 1);
    }

    int length = get_int_input("Enter sequence length: ");

    if (length <= 0)
    {
        return new lazy_sequence<T>(nullptr, gen);
    }
    else
    {
        T *init_items = new T[length];
        for (int i = 0; i < length; i++)
        {
            init_items[i] = gen->get_next();
        }
        lazy_sequence<T> *seq = new lazy_sequence<T>(init_items, length);
        delete[] init_items;
        seq->set_generator(gen);
        return seq;
    }
}

template <typename T>
void print_lazy_sequence(lazy_sequence<T> *seq, int max_items = 20)
{
    std::cout << "[";
    try
    {
        for (int i = 0; i < max_items; i++)
        {
            if (i > 0)
                std::cout << ", ";
            std::cout << seq->get(i);
        }
        std::cout << " ...]" << std::endl;
    }
    catch (...)
    {
        std::cout << "]" << std::endl;
    }
}

template <typename T>
void run_lazy_sequence_menu()
{
    lazy_sequence<T> *seq = nullptr;
    lazy_sequence<T> *other_seq = nullptr;

    while (true)
    {
        std::cout << "\n=== Lazy Sequence Operations Menu ===" << std::endl;
        std::cout << "1. Create new lazy_sequence" << std::endl;
        std::cout << "2. Get element by index" << std::endl;
        std::cout << "3. Get first element" << std::endl;
        std::cout << "4. Get next element" << std::endl;
        std::cout << "5. Get last memoized element" << std::endl;
        std::cout << "6. Get subsequence" << std::endl;
        std::cout << "7. Concatenate with another sequence" << std::endl;
        std::cout << "8. Apply map (transformation)" << std::endl;
        std::cout << "9. Apply where (filtering)" << std::endl;
        std::cout << "10. Set element at index" << std::endl;
        std::cout << "11. Set first element" << std::endl;
        std::cout << "12. Set last memoized element" << std::endl;
        std::cout << "13. Reduce (fold)" << std::endl;
        std::cout << "14. Show sequence" << std::endl;
        std::cout << "15. Create second sequence for operations" << std::endl;
        std::cout << "16. Return to main menu" << std::endl;
        std::cout << "0. Exit" << std::endl;

        int choice = get_int_input("Choose operation: ");

        try
        {
            switch (choice)
            {
            case 1:
                if (seq)
                    delete seq;
                seq = create_lazy_sequence<T>();
                std::cout << "Lazy sequence created." << std::endl;
                break;

            case 2:
                if (seq)
                {
                    int index = get_int_input("Enter index: ");
                    std::cout << "Element [" << index << "] = " << seq->get(index) << std::endl;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 3:
                if (seq)
                {
                    std::cout << "First element: " << seq->get_first() << std::endl;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 4:
                if (seq)
                {
                    // get_next is not available in header
                    // Instead get current length and next element
                    std::cout << "Next element: " << seq->get_next() << std::endl;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 5:
                if (seq)
                {
                    std::cout << "Last memoized element: " << seq->get_last_memoized() << std::endl;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 6:
                if (seq)
                {
                    int start = get_int_input("Enter start index: ");
                    int end = get_int_input("Enter end index: ");
                    lazy_sequence<T> *subseq = seq->get_subsequence(start, end);
                    std::cout << "Subsequence [" << start << ":" << end << "]:" << std::endl;
                    print_lazy_sequence(subseq);
                    // Note: don't forget to delete subseq when done
                    delete subseq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 7:
                if (seq)
                {
                    if (!other_seq)
                    {
                        std::cout << "Please create second sequence first (option 15)!" << std::endl;
                    }
                    else
                    {
                        lazy_sequence<T> *concated = seq->concat(*other_seq);
                        std::cout << "Concatenation result:" << std::endl;
                        print_lazy_sequence(concated);
                        delete concated;
                    }
                }
                else
                {
                    std::cout << "Please create main sequence first!" << std::endl;
                }
                break;

            case 8:
                if (seq)
                {
                    std::cout << "Choose transformation:" << std::endl;
                    std::cout << "1. Multiply by 2" << std::endl;
                    std::cout << "2. Square" << std::endl;
                    std::cout << "3. Increment" << std::endl;

                    int map_choice = get_int_input("Your choice: ");

                    std::function<T(T)> mapper;
                    switch (map_choice)
                    {
                    case 1:
                        mapper = [](T x)
                        { return x * 2; };
                        break;
                    case 2:
                        mapper = [](T x)
                        { return x * x; };
                        break;
                    case 3:
                        mapper = [](T x)
                        { return x + 1; };
                        break;
                    default:
                        std::cout << "Invalid choice!" << std::endl;
                        continue;
                    }

                    auto mapped_seq = seq->map<T>(mapper);
                    std::cout << "Map result:" << std::endl;
                    print_lazy_sequence(mapped_seq);
                    delete mapped_seq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 9:
                if (seq)
                {
                    std::cout << "Choose filter:" << std::endl;
                    std::cout << "1. Even numbers" << std::endl;
                    std::cout << "2. Odd numbers" << std::endl;
                    std::cout << "3. Greater than 10" << std::endl;

                    int filter_choice = get_int_input("Your choice: ");

                    std::function<bool(T)> filter;
                    switch (filter_choice)
                    {
                    case 1:
                        filter = [](T x)
                        { return static_cast<int>(x) % 2 == 0; };
                        break;
                    case 2:
                        filter = [](T x)
                        { return static_cast<int>(x) % 2 != 0; };
                        break;
                    case 3:
                        filter = [](T x)
                        { return x > 10; };
                        break;
                    default:
                        std::cout << "Invalid choice!" << std::endl;
                        continue;
                    }

                    auto filtered_seq = seq->where(filter);
                    std::cout << "Where result:" << std::endl;
                    print_lazy_sequence(filtered_seq);
                    delete filtered_seq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 10:
                if (seq)
                {
                    int index = get_int_input("Enter index: ");
                    T value;
                    std::cout << "Enter new value: ";
                    std::cin >> value;
                    clear_input();

                    lazy_sequence<T> *new_seq = seq->set(value, index);
                    std::cout << "Element set." << std::endl;
                    print_lazy_sequence(new_seq);
                    delete new_seq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 11:
                if (seq)
                {
                    T value;
                    std::cout << "Enter new value for first element: ";
                    std::cin >> value;
                    clear_input();

                    lazy_sequence<T> *new_seq = seq->set_first(value);
                    std::cout << "First element set." << std::endl;
                    print_lazy_sequence(new_seq);
                    delete new_seq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 12:
                if (seq)
                {
                    T value;
                    std::cout << "Enter new value for last memoized element: ";
                    std::cin >> value;
                    clear_input();

                    lazy_sequence<T> *new_seq = seq->set_last_memoized(value);
                    std::cout << "Last memoized element set." << std::endl;
                    print_lazy_sequence(new_seq);
                    delete new_seq;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 13:
                if (seq)
                {
                    std::cout << "Choose reduce operation:" << std::endl;
                    std::cout << "1. Sum" << std::endl;
                    std::cout << "2. Product" << std::endl;
                    std::cout << "3. Maximum" << std::endl;

                    int reduce_choice = get_int_input("Your choice: ");

                    std::function<T(T, T)> reducer;
                    T initial;

                    switch (reduce_choice)
                    {
                    case 1:
                        reducer = [](T a, T b)
                        { return a + b; };
                        initial = 0;
                        break;
                    case 2:
                        reducer = [](T a, T b)
                        { return a * b; };
                        initial = 1;
                        break;
                    case 3:
                        reducer = [](T a, T b)
                        { return a > b ? a : b; };
                        initial = std::numeric_limits<T>::min();
                        break;
                    default:
                        std::cout << "Invalid choice!" << std::endl;
                        continue;
                    }

                    T result = seq->reduce(reducer, initial);
                    std::cout << "Reduce result: " << result << std::endl;
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 14:
                if (seq)
                {
                    std::cout << "Current sequence:" << std::endl;
                    print_lazy_sequence(seq);
                }
                else
                {
                    std::cout << "Please create a sequence first!" << std::endl;
                }
                break;

            case 15:
                if (other_seq)
                    delete other_seq;
                other_seq = create_lazy_sequence<T>();
                std::cout << "Second sequence created." << std::endl;
                break;

            case 16:
                if (seq)
                    delete seq;
                if (other_seq)
                    delete other_seq;
                return;

            case 0:
                if (seq)
                    delete seq;
                if (other_seq)
                    delete other_seq;
                exit(0);

            default:
                std::cout << "Invalid choice!" << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
}

void main_menu()
{
    while (true)
    {
        std::cout << "\n=== MAIN MENU ===" << std::endl;
        std::cout << "1. Work with lazy_sequence" << std::endl;
        std::cout << "2. Ordinal arithmetic" << std::endl;
        std::cout << "0. Exit" << std::endl;

        int choice = get_int_input("Choose operation type: ");

        switch (choice)
        {
        case 1:
        {
            std::cout << "\nChoose data type for lazy_sequence:" << std::endl;
            std::cout << "1. int" << std::endl;
            std::cout << "2. double" << std::endl;
            std::cout << "3. Go back" << std::endl;

            int type_choice = get_int_input("Your choice: ");

            switch (type_choice)
            {
            case 1:
                run_lazy_sequence_menu<int>();
                break;
            case 2:
                run_lazy_sequence_menu<double>();
                break;
            case 3:
                break;
            default:
                std::cout << "Invalid choice!" << std::endl;
            }
            break;
        }

        case 2:
            run_ordinal_menu();
            break;

        case 0:
            exit(0);

        default:
            std::cout << "Invalid choice!" << std::endl;
        }
    }
}

int main()
{
    std::cout << "Program for working with lazy_sequence and ordinals" << std::endl;
    main_menu();
    return 0;
}
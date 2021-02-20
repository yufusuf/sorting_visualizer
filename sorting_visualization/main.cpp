#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <windows.h>


void swap_(int arr[], int, int);
void bubble_sort(int bars[], int size);
void drawBars(sf::RenderWindow& window, int heights[]);
void quick_sort(int bars[], int first, int last);
void shuffleArray(int bars[]);

static const int WINDOW_HEIGHT = 512;
static const int WINDOW_WIDTH = 1024;
static const float bar_width = 8.0f;
int bars_count = WINDOW_WIDTH / bar_width;
int low, high;

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting Visualizer", sf::Style::Resize | sf::Style::Close);

void drawBars(sf::RenderWindow& window, int heights[])
{
    window.clear();
    for (int x = 0; x < bars_count; x++)
    {
        int height = heights[x];
        sf::RectangleShape bar(sf::Vector2f(bar_width, height));
        if (x == low || x == high)
            bar.setFillColor(sf::Color::Green);

        bar.setPosition(sf::Vector2f(bar_width * x, WINDOW_HEIGHT - bar.getSize().y));
        window.draw(bar);
    }
    window.display();
}

void shuffleArray(int bars[])
{


    for (int k = bars_count - 1; k >= 1; k--)
    {
        int random = rand() % k;
        swap_(bars, random, k);
    }
}

void swap_(int bars[], int index1, int index2)
{
    std::swap(bars[index1], bars[index2]);
    drawBars(window, bars);
    //Sleep(10);
}


void bubble_sort(int bars[], int size)
{
    bool swapped = true;
    while (swapped)
    {
        swapped = false;
        for (int i = 1; i < size; i++)
        {
            if (bars[i - 1] > bars[i])
            {
                swap_(bars, i, i - 1);
                swapped = true;
            }
        }

    }
}

int partition(int bars[], int first, int last)
{
    // LOMUTO
    /*int pivot = bars[last];
    int i = first - 1;
    for (int j = first ; j < last; j++)
    {
        if (bars[j] < pivot)
        {
            i++;
            swap_(bars, i, j);
        }
    }
    swap_(bars, i+1, last);
    return i + 1;*/

    // HOARE
    int pivot = bars[(int)std::floor((first + last) / 2)];
    int i = first - 1;
    int j = last + 1;
    while (true)
    {
        do
        {
            i++;
        } while (bars[i] < pivot);
        do
        {
            j--;
        } while (bars[j] > pivot);
        if (i >= j)
            return j;
        swap_(bars, i, j);
    }
}

void quick_sort(int bars[], int first, int last)
{
    if (first < last)
    {
        low = first;
        high = last;
        int pivot = partition(bars, first, last);
        quick_sort(bars, first, pivot);
        quick_sort(bars, pivot + 1, last);
    }

}

int main()
{
    // ===== CONSTRUCTUING BARS ==== //
    srand(time(NULL));
    int* heights = new int[bars_count];

    for (int i = 0; i < bars_count; i++)
    {
        heights[i] = rand() % WINDOW_HEIGHT;

    }

    drawBars(window, heights);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            //shuffle array
            shuffleArray(heights);
            quick_sort(heights, 0, bars_count - 1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            shuffleArray(heights);
            bubble_sort(heights, bars_count);
        }

    }


    return 0;
}
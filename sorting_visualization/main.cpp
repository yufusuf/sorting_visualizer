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
void merge_sort(int bars[], int start, int end);
void merge_arrays(int bars[], int start, int mid ,int end);
void heap_sort(int bars[], int size);
void percolate_down(int bars[], int index, int size);

static const int WINDOW_HEIGHT = 512;
static const int WINDOW_WIDTH = 1024;
static const float bar_width = 2.0f;
static const int bars_count = WINDOW_WIDTH / bar_width;


sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Sorting Visualizer", sf::Style::Resize | sf::Style::Close);

void drawBars(sf::RenderWindow& window, int heights[])
{
    window.clear();
    for (int x = 0; x < bars_count; x++)
    {
        int height = heights[x];
        sf::RectangleShape bar(sf::Vector2f(bar_width, height));
        //if (x == low || x == high)
        //    bar.setFillColor(sf::Color::Green);

        bar.setPosition(sf::Vector2f(bar_width * x, WINDOW_HEIGHT - bar.getSize().y));
        window.draw(bar);
    }
    window.display();
}

void shuffleArray(int bars[])
{
    std::cout << "Shuffling " << std::endl;
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
    //Sleep(1);
}

void percolate_down(int bars[], int index, int size)
{
    int child;
    int temp = bars[index];

    for (; 2* index < size - 1 ; index = child)
    {
        child = 2 * index + 1;
        if (child != size - 1 && bars[child] < bars[child + 1])
          //^^(single child case)
        {
           
            child++;
        }
        if (bars[child] > temp)
        {
            bars[index] = bars[child];
        }
        else break; // largest is parent, done

    }
    bars[index] = temp;
    
    //int left = 2 * index + 1;
    //int right = 2 * index + 2;
    //int largest = index;
    //if (left < size && bars[left] > bars[largest])
    //{
    //    largest = left;
    //}
    //if (right < size && bars[right] > bars[largest])
    //{
    //    largest = right;
    //}
    //if (largest != index)
    //{
    //    swap_(bars, index, largest);
    //    percolate_down(bars, largest, size);
    //}
}

void heap_sort(int bars[], int size)
{
    //heapify the bars
    std::cout << "Building heap..." << std::endl;

    for(int i = size/2 - 1; i>=0; i--)
    {
        percolate_down(bars, i, size);
        drawBars(window, bars);
    }
    std::cout << "Sorting" << std::endl;
    for (int j = size - 1; j > 0; j--)
    {
        swap_(bars, 0, j);
        percolate_down(bars, 0, j);
    }
}

void merge_arrays(int bars[], int start, int mid, int end)
{
    int index = start;
    int start1 = start;
    int start2 = mid + 1;
    int last1 = mid;
    int last2 = end;
    int *temp = new int[bars_count];

    for (; (start1 <= last1) && (start2 <= last2); index++)
    {
        if (bars[start1] <= bars[start2])
        {
            temp[index] = bars[start1];
            start1++;
        }
        else
        {
            temp[index] = bars[start2];
            start2++;
        }
    }

    for(; start2 <= last2; start2++,index++)
    {
        temp[index] = bars[start2];
    }
    for (; start1 <= last1; start1++,index++)
    {
        temp[index] = bars[start1];
    }
    for (index = start; index <= end; index++)
    {
        bars[index] = temp[index];
        drawBars(window, bars);
    }
    //Sleep(10);
   /* drawBars(window, bars);*/
    delete[] temp;
}

void merge_sort(int bars[], int start, int end)
{
    if (start >= end)
        return;
    int mid = (start + end) / 2;
  
    merge_sort(bars, start, mid);
    merge_sort(bars, mid + 1, end);

    merge_arrays(bars, start, mid, end);
    
    
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
        size--;
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
        //low = first;q
        //high = last;
        int pivot = partition(bars, first, last);
        quick_sort(bars, first, pivot);
        quick_sort(bars, pivot + 1, last);
    }

}

int main()
{
    // ===== CONSTRUCTUING BARS ==== //
    srand(time(NULL));
    bool isSorted = false;
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
            if (isSorted) 
                shuffleArray(heights); 
            std::cout << "Quick sort" << std::endl;
            quick_sort(heights, 0, bars_count - 1);
            isSorted = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        {
            if (isSorted) shuffleArray(heights);
            std::cout << "Bubble sort" << std::endl;
            bubble_sort(heights, bars_count);
            isSorted = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        {
            if (isSorted) shuffleArray(heights);
            std::cout << "Merge sort" << std::endl;
            merge_sort(heights, 0, bars_count-1);
            isSorted = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            if (isSorted) shuffleArray(heights);
            heap_sort(heights, bars_count);
            isSorted = true;
        }
        drawBars(window, heights);
    }

    delete[] heights;


    return 0;
}
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

//locks[i]is all the candidate who "locks" i candidate
int locks[MAX][MAX];

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void merge_pairs(int l, int r);
void lock_pairs(void);
void print_winner(void);
bool check_if_cycle(int l, int w);


int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                j--;
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(candidates[i], name) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    int k = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] >  preferences[j][i])
            {
                pair p;
                p.winner = i;
                p.loser = j;
                pairs[k++] = p;
                preferences[i][j] -= preferences[j][i];
            }
            else if (preferences[j][i] > preferences[i][j])
            {
                pair p = {j, i};
                pairs[k++] = p;
                preferences[j][i] -= preferences[i][j];
            }
        }
    }
    pair_count = k;
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs_helper(int l, int r)
{
    if (l >= r)
    {
        return;
    }
    int mid = l + (r - l) / 2;
    sort_pairs_helper(l, mid);
    sort_pairs_helper(mid + 1, r);
    merge_pairs(l, r);
}
// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    sort_pairs_helper(0, pair_count - 1);
}

//merge two arrays
void merge_pairs(int l, int r)
{
    int mid = l + (r - l) / 2;
    int n1  = mid - l + 1;
    int n2 = r - mid;
    //create temp arrays
    pair p1[n1], p2[n2];

    //copy data to temp arrays
    for (int i = 0; i < n1; i++)
    {
        p1[i] = pairs[l + i];
    }
    for (int i = 0; i < n2; i++)
    {
        p2[i] = pairs[mid + 1 + i];
    }

    //merge back into pairs;
    int i = 0;
    int j = 0;
    int k = l;
    while (i < n1 && j < n2)
    {
        if (preferences[p1[i].winner][p1[i].loser] > preferences[p2[j].winner][p2[j].loser])
        {
            pairs[k] = p1[i++];
        }
        else
        {
            pairs[k] = p2[j++];
        }
        k++;
    }

    while (i < n1)
    {
        pairs[k++] = p1[i++];
    }
    while (j < n2)
    {
        pairs[k++] = p2[j++];
    }

}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_if_cycle(pairs[i].loser, pairs[i].winner))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

//check if there is a path from loser to winner
bool check_if_cycle(int l, int w)
{
    if (locked[l][w] == true)
    {
        return true;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        //if clocked[i][w], i is winner of w
        if (locked[i][w] == true)
        {
            //check if l is winner of i
            if (check_if_cycle(l, i))
            {
                return true;
            };
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int coun = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                coun++;
                break;
            }
        }
        if (coun == 0)
        {
            printf("%s\n", candidates[i]);
        }
    }
    return;
}


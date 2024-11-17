#include<bits/stdc++.h>
using namespace std;

// Function to calculate the support count of itemsets
vector<set<string>> calculateSupport(vector<set<string>>& transactions, vector<set<string>>& candidates, int minSupport, map<set<string>, int>& supportCount) {
    vector<set<string>> frequentItemsets;

    for (auto& candidate : candidates) {
        int cnt = 0;
        for (auto& transaction : transactions) {
            if (includes(transaction.begin(), transaction.end(), candidate.begin(), candidate.end())) {
                cnt++;
            }
        }

        if (cnt >= minSupport) {
            frequentItemsets.push_back(candidate);
            supportCount[candidate] = cnt;
        }
    }

    return frequentItemsets;
}

// Function to generate candidates of the next level from frequent itemsets
vector<set<string>> generateCandidates(vector<set<string>>& frequentItemsets) {
    vector<set<string>> candidates;

    for (size_t i = 0; i < frequentItemsets.size(); i++) {
        for (size_t j = i + 1; j < frequentItemsets.size(); j++) {
            set<string> candidate(frequentItemsets[i]);
            candidate.insert(frequentItemsets[j].begin(), frequentItemsets[j].end());

            if (candidate.size() == frequentItemsets[0].size() + 1) {
                candidates.push_back(candidate);
            }
        }
    }

    // Remove duplicate candidates
    sort(candidates.begin(), candidates.end());
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    return candidates;
}

// Function to read transactions from a text file
vector<set<string>> readTransactions(const string& filename) {
    vector<set<string>> transactions;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        set<string> transaction;
        stringstream ss(line);
        string item;

        while (getline(ss, item, ',')) {
            transaction.insert(item);
        }

        transactions.push_back(transaction);
    }

    file.close();
    return transactions;
}


int main() {

    ///freopen ("output.txt","w",stdout);

    string filename = "transactions.txt";
    vector<set<string>> transactions = readTransactions(filename);

    // Minimum support threshold
    int minSupport = 2;

    // Generate frequent itemsets of size 1
    set<string> items;
    for (auto& transaction : transactions) {
        items.insert(transaction.begin(), transaction.end());
    }

    vector<set<string>> candidates;
    for (auto& item : items) {
        candidates.push_back({item});
    }

    map<set<string>, int> supportCount;
    vector<set<string>> frequentItemsets = calculateSupport(transactions, candidates, minSupport, supportCount);

    // Iteratively find frequent itemsets of larger sizes
    while (!frequentItemsets.empty()) {
        cout << "Frequent Itemsets of size " << frequentItemsets[0].size() << ":\n";
        for (auto& itemset : frequentItemsets) {
            cout << "{ ";
            for (auto& item : itemset) {
                cout << item << " ";
            }
            cout << "} : " << supportCount[itemset] << "\n";
        }

        // Generate candidates for the next level
        candidates = generateCandidates(frequentItemsets);

        // Calculate support for the new candidates
        frequentItemsets = calculateSupport(transactions, candidates, minSupport, supportCount);
    }

    return 0;
}

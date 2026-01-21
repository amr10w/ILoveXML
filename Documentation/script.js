// ====================
// SEARCH INDEX
// ====================
// This index maps method/function names to their pages for quick searching
const searchIndex = {
    // BPE.html
    'BPE': { page: 'BPE.html', type: 'class', description: 'Byte Pair Encoding class' },
    'train': { page: 'BPE.html', type: 'method', description: 'Train the BPE model' },
    'encode': { page: 'BPE.html', type: 'method', description: 'Encode text using BPE' },
    'decode': { page: 'BPE.html', type: 'method', description: 'Decode BPE-encoded text' },

    // Compress.html
    'compressFile': { page: 'Compress.html', type: 'function', description: 'Compress a file using BPE' },
    'compressString': { page: 'Compress.html', type: 'function', description: 'Compress a string' },

    // Decompress.html
    'decompressFile': { page: 'Decompress.html', type: 'function', description: 'Decompress a BPE file' },
    'decompressString': { page: 'Decompress.html', type: 'function', description: 'Decompress a string' },

    // FileUtils.html
    'readFile': { page: 'FileUtils.html', type: 'function', description: 'Read file contents' },
    'writeFile': { page: 'FileUtils.html', type: 'function', description: 'Write to file' },
    'tokenize': { page: 'FileUtils.html', type: 'function', description: 'Tokenize text' },
    'split': { page: 'FileUtils.html', type: 'function', description: 'Split string by delimiter' },

    // Graph.html
    'Graph': { page: 'Graph.html', type: 'class', description: 'Social network graph class' },
    'addEdge': { page: 'Graph.html', type: 'method', description: 'Add edge to graph' },
    'getNeighbors': { page: 'Graph.html', type: 'method', description: 'Get user neighbors' },
    'getMostInfluencerId': { page: 'Graph.html', type: 'method', description: 'Find most influential user' },
    'getMostActivePersonId': { page: 'Graph.html', type: 'method', description: 'Find most active user' },

    // GraphVisualizer.html
    'visualize': { page: 'GraphVisualizer.html', type: 'function', description: 'Visualize graph structure' },

    // GraphvizWrapper.html
    'renderGraph': { page: 'GraphvizWrapper.html', type: 'function', description: 'Render graph using Graphviz' },

    // JsonConverter.html
    'toJson': { page: 'JsonConverter.html', type: 'function', description: 'Convert XML to JSON' },

    // Mutual.html
    'getMutualFollowers': { page: 'Mutual.html', type: 'function', description: 'Find mutual followers' },

    // SearchTopic.html
    'searchByTopic': { page: 'SearchTopic.html', type: 'function', description: 'Search posts by topic' },

    // SearchWord.html
    'searchByWord': { page: 'SearchWord.html', type: 'function', description: 'Search posts by word' },

    // SimpleMap.html
    'SimpleMap': { page: 'SimpleMap.html', type: 'class', description: 'Custom hash map implementation' },
    'insert': { page: 'SimpleMap.html', type: 'method', description: 'Insert key-value pair' },
    'get': { page: 'SimpleMap.html', type: 'method', description: 'Get value by key' },
    'remove': { page: 'SimpleMap.html', type: 'method', description: 'Remove entry' },

    // Suggest.html
    'suggest': { page: 'Suggest.html', type: 'function', description: 'Suggest friends' },
    'printSuggestions': { page: 'Suggest.html', type: 'function', description: 'Print suggestions' },

    // Tree.html
    'Tree': { page: 'Tree.html', type: 'class', description: 'Generic tree structure' },
    'insert': { page: 'Tree.html', type: 'method', description: 'Insert node' },
    'traverse': { page: 'Tree.html', type: 'method', description: 'Traverse tree' },

    // XMLMinifier.html
    'minify': { page: 'XMLMinifier.html', type: 'function', description: 'Minify XML' },

    // XMLValidator.html
    'validate': { page: 'XMLValidator.html', type: 'function', description: 'Validate XML' },
    'fix': { page: 'XMLValidator.html', type: 'function', description: 'Fix XML errors' },
    'getErrors': { page: 'XMLValidator.html', type: 'function', description: 'Get validation errors' }
};

// ====================
// SEARCH FUNCTIONALITY
// ====================
let selectedSearchIndex = -1;
let currentSearchResults = [];

function performSearch() {
    const input = document.getElementById('classSearch');
    const query = input.value.trim();
    const resultsContainer = getOrCreateSearchResults();

    // Clear previous results
    resultsContainer.innerHTML = '';
    currentSearchResults = [];
    selectedSearchIndex = -1;

    if (query.length === 0) {
        resultsContainer.classList.remove('show');
        // Also filter sidebar
        filterClasses();
        return;
    }

    // Search in the index
    const matches = [];
    const queryLower = query.toLowerCase();

    for (const [name, info] of Object.entries(searchIndex)) {
        if (name.toLowerCase().includes(queryLower)) {
            matches.push({ name, ...info });
        }
    }

    // Display results
    if (matches.length > 0) {
        matches.forEach((match, index) => {
            const item = document.createElement('div');
            item.className = 'search-result-item';
            item.setAttribute('data-index', index);
            const pageName = match.page.replace('.html', '');
            item.innerHTML = `
                <div class="search-result-page">${pageName}</div>
                <div class="search-result-name">${match.name}</div>
            `;
            item.onclick = () => navigateToPage(match.page);
            item.onmouseenter = () => {
                selectedSearchIndex = index;
                updateSelectedResult();
            };
            resultsContainer.appendChild(item);
        });
        currentSearchResults = matches;
        resultsContainer.classList.add('show');
    } else {
        resultsContainer.innerHTML = '<div class="search-no-results">No results found</div>';
        resultsContainer.classList.add('show');
    }

    // Also filter sidebar
    filterClasses();
}

function getOrCreateSearchResults() {
    let container = document.querySelector('.search-results');
    if (!container) {
        container = document.createElement('div');
        container.className = 'search-results';
        document.querySelector('.search-container').appendChild(container);
    }
    return container;
}

function navigateToPage(page) {
    window.location.href = page;
}

function updateSelectedResult() {
    const items = document.querySelectorAll('.search-result-item');
    items.forEach((item, index) => {
        if (index === selectedSearchIndex) {
            item.classList.add('selected');
        } else {
            item.classList.remove('selected');
        }
    });
}

function handleSearchKeydown(event) {
    const resultsContainer = document.querySelector('.search-results');
    if (!resultsContainer || !resultsContainer.classList.contains('show')) {
        return;
    }

    switch (event.key) {
        case 'ArrowDown':
            event.preventDefault();
            selectedSearchIndex = Math.min(selectedSearchIndex + 1, currentSearchResults.length - 1);
            updateSelectedResult();
            break;
        case 'ArrowUp':
            event.preventDefault();
            selectedSearchIndex = Math.max(selectedSearchIndex - 1, 0);
            updateSelectedResult();
            break;
        case 'Enter':
            event.preventDefault();
            if (selectedSearchIndex >= 0 && currentSearchResults[selectedSearchIndex]) {
                navigateToPage(currentSearchResults[selectedSearchIndex].page);
            }
            break;
        case 'Escape':
            resultsContainer.classList.remove('show');
            break;
    }
}

// Close search results when clicking outside
document.addEventListener('click', function (event) {
    const searchContainer = document.querySelector('.search-container');
    if (searchContainer && !searchContainer.contains(event.target)) {
        const resultsContainer = document.querySelector('.search-results');
        if (resultsContainer) {
            resultsContainer.classList.remove('show');
        }
    }
});

// ====================
// SIDEBAR FILTERING
// ====================
function filterClasses() {
    // Get search input and list
    var input = document.getElementById('classSearch');
    var filter = input.value.toUpperCase();
    var sidebar = document.getElementById('sidebar');
    var links = sidebar.getElementsByTagName('a');
    var headers = document.querySelectorAll('.sidebar-category');

    // Loop through all list items (links), and hide those who don't match the search query
    // Also handling category headers visibility implies a more complex logic, 
    // but for simplicity we will just filter the links. Categories might remain empty visually.

    for (var i = 0; i < links.length; i++) {
        var txtValue = links[i].textContent || links[i].innerText;
        if (txtValue.toUpperCase().indexOf(filter) > -1) {
            links[i].style.display = "";
            links[i].parentElement.style.display = ""; // Ensure the list item is visible
        } else {
            links[i].style.display = "none";
            links[i].parentElement.style.display = "none"; // Hide list item to avoid spacing issues
        }
    }
}

// ====================
// ACTIVE PAGE HIGHLIGHTING
// ====================
// Automatically highlight the active page in the sidebar
function setActivePage() {
    // Get the current page filename
    var currentPage = window.location.pathname.split('/').pop();

    // If no page specified (root or empty), default to index.html
    if (!currentPage || currentPage === '') {
        currentPage = 'index.html';
    }

    // Get all sidebar links
    var sidebar = document.getElementById('sidebar');
    if (!sidebar) return;

    var links = sidebar.getElementsByTagName('a');

    // Remove any existing active-page classes and add to matching link
    for (var i = 0; i < links.length; i++) {
        var link = links[i];
        var href = link.getAttribute('href');

        // Remove active class from all links first
        link.classList.remove('active-page');

        // Check if this link matches the current page
        if (href === currentPage) {
            link.classList.add('active-page');

            // Scroll the active link into view if needed
            link.scrollIntoView({ behavior: 'smooth', block: 'nearest' });
        }
    }
}

// ====================
// INITIALIZATION
// ====================
// Run when the DOM is fully loaded
document.addEventListener('DOMContentLoaded', function () {
    setActivePage();

    // Set up search input
    const searchInput = document.getElementById('classSearch');
    if (searchInput) {
        // Change from onkeyup to oninput for better responsiveness
        searchInput.oninput = performSearch;
        searchInput.onkeydown = handleSearchKeydown;
    }
});

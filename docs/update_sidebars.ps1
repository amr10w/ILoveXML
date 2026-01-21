# Script to update all HTML files with the new sidebar that includes Overview

$htmlFiles = Get-ChildItem -Path "c:\Users\Amr\Desktop\Study\DSA Project\XML-Social-Network-Analyzer\docs\html\*.html" -File

foreach ($file in $htmlFiles) {
    $content = Get-Content $file.FullName -Raw
    
    # Find and replace the sidebar section
    # Pattern: from <aside class="sidebar" to </aside>
    $pattern = '(?s)(<aside class="sidebar"[^>]*>)(.*?)(</aside>)'
    
    $newSidebar = @'
<aside class="sidebar" id="sidebar">
    <h3>Documentation</h3>

    <div class="sidebar-category">Overview</div>
    <ul>
        <li><a href="index.html">Overview</a></li>
    </ul>

    <div class="sidebar-category">Core (Backend)</div>
    <ul id="coreList">
        <li><a href="BPE.html">BPE</a></li>
        <li><a href="Compress.html">Compress</a></li>
        <li><a href="Decompress.html">Decompress</a></li>
        <li><a href="FileUtils.html">FileUtils</a></li>
        <li><a href="Graph.html">Graph</a></li>
        <li><a href="GraphVisualizer.html">GraphVisualizer</a></li>
        <li><a href="GraphvizWrapper.html">GraphvizWrapper</a></li>
        <li><a href="JsonConverter.html">JsonConverter</a></li>
        <li><a href="Mutual.html">Mutual</a></li>
        <li><a href="SearchTopic.html">SearchTopic</a></li>
        <li><a href="SearchWord.html">SearchWord</a></li>
        <li><a href="SimpleMap.html">SimpleMap</a></li>
        <li><a href="Suggest.html">Suggest</a></li>
        <li><a href="Tree.html">Tree</a></li>
        <li><a href="XMLMinifier.html">XMLMinifier</a></li>
        <li><a href="XMLValidator.html">XMLValidator</a></li>
    </ul>

    <div class="sidebar-category">CLI</div>
    <ul id="cliList">
        <li><a href="CLICommands.html">CLICommands</a></li>
    </ul>

    <div class="sidebar-category">UI (Qt/C++)</div>
    <ul id="uiList">
        <li><a href="mainwindow.html">mainwindow</a></li>
        <li><a href="AboutPage.html">AboutPage</a></li>
        <li><a href="CompressPage.html">CompressPage</a></li>
        <li><a href="DecompressPage.html">DecompressPage</a></li>
        <li><a href="formattingPage.html">formattingPage</a></li>
        <li><a href="graphPage.html">graphPage</a></li>
    </ul>
</aside>
'@
    
    if ($content -match $pattern) {
        $updatedContent = $content -replace $pattern, $newSidebar
        Set-Content -Path $file.FullName -Value $updatedContent -NoNewline
        Write-Host "Updated: $($file.Name)"
    } else {
        Write-Host "Skipped (no sidebar found): $($file.Name)"
    }
}

Write-Host "`nDone updating all HTML files!"

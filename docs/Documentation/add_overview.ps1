# Script to add Overview link to all HTML documentation sidebars
$docsPath = "c:\Users\Amr\Desktop\Study\DSA Project\XML-Social-Network-Analyzer\docs\html"
$htmlFiles = Get-ChildItem -Path $docsPath -Filter "*.html" -Exclude "sidebar_snippet.html"

$overviewLink = '            <a href="index.html" class="sidebar-overview">📋 Overview</a>'

foreach ($file in $htmlFiles) {
    $content = Get-Content $file.FullName -Raw
    
    # Check if <h3>Documentation</h3> exists and Overview not already added
    if ($content -match '<h3>Documentation</h3>' -and $content -notmatch 'sidebar-overview') {
        # Add the Overview link right after <h3>Documentation</h3>
        $content = $content -replace '(<h3>Documentation</h3>)', "`$1`r`n$overviewLink"
        
        # Write back to file
        Set-Content -Path $file.FullName -Value $content -NoNewline
        Write-Host "✓ Updated: $($file.Name)"
    }
}

Write-Host "`n✅ All files updated!"

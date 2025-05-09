$countFile = "$PSScriptRoot\buildcount.txt"
$count = [int](Get-Content $countFile -ErrorAction SilentlyContinue) + 1
$count | Out-File $countFile
Write-Host "This is build #$count"
$data = dir './' -filter *.bf -recurse
$data | Foreach-Object{
  echo $_.Name
  '../release/bl' $_.Name
}
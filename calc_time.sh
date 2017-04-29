start_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')
./knapsack -i examples/1000_50_800.in -bb -t 20
end_ms=$(ruby -e 'puts (Time.now.to_f * 1000).to_i')
elapsed_ms=$((end_ms - start_ms))
echo "$elapsed_ms ms passed"
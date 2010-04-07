run=/d01/frankma/scratch/pp/dNdEta/anaCMSSW_3_5_6

# bambu
cp -v $run/bambu/*.C bambu/

# pixel tree
cp -v $run/diffractionAna/pixel_trees/*.C pixeltree/

# macros
cp -v $run/diffractionAna/macros/*.C macros/

# analysis
cp -v $run/diffractionAna/collbx_7TeV/*.h ana/
cp -v $run/diffractionAna/collbx_7TeV/*.C ana/

# results
cp -v -r $run/diffractionAna/collbx_7TeV/results ana/

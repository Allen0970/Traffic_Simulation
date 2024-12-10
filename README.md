# Simulation_Prediction

## Realistic Event Data Generation Procedure

##### 1. Install SUMO
Install the [SUMO](https://sumo.dlr.de/docs/Downloads.php) simulator on your system.

##### 2. Select Region in OpenStreetMap
- Go to [OpenStreetMap](https://www.openstreetmap.org/#map=5/38.01/-95.84) and select the region you want to simulate.
- Export the region as a `.osm` file.
- Manhattan road network in this work can be found in Google Drive named `map.osm`.

##### 3. Copy the Typemap File and Convert OSM to SUMO Network
- Copy the `osmNetconvert.typ.xml` file from the `$SUMO_HOME/data/typemap` directory.
- Open a terminal in the directory where your `.osm` file is located.
- Run the following command to convert the `.osm` file into a `.xml` file that can be loaded into SUMO:

```bash
netconvert --osm-files map.osm --keep-edges.by-vclass passenger --keep-edges.by-type highway.living_street,highway.motorway,highway.motorway_link,highway.primary,highway.primary_link,highway.residential,highway.secondary,highway.secondary_link,highway.tertiary,highway.tertiary_link,highway.trunk,highway.trunk_link,highway.unclassified -o test.net.xml --no-internal-links -t osmNetconvert.typ.xml --keep-edges.components 1
```

##### 4. Count the Number of Roads and Connections
- Run the following command:
  
```bash
python count_edges_and_nodes.py --input test.net.xml
```

##### 5. Add Polygons to the Simulation Procedure
- Save [polygon information](https://sumo.dlr.de/wiki/Networks/Import/OpenStreetMap) into file `typemap.xml`
- Run the following command:

```bash
polyconvert --net-file test.net.xml --osm-files map.osm --type-file typemap.xml -o map.poly.xml
```

##### 6. Generate Route Data as Input in SUMO
- Limited to data size, it can be download from GoogleDrive.
- Run the following command:

```bash
python rerouteTrips.py --input test.net.xml
```

##### 7. SUMO Simualation to Generate Route Data with Detailed Traveling Behaviors
- Create and edit `map.sumo.cfg` ([Example](https://github.com/eclipse/sumo/blob/main/tests/complex/tutorial/hello/data/hello.sumocfg)).
- For SUMO simulation without detailed traveling behaviors information, run the following command:

```bash
sumo-gui -c map.sumo.cfg 
```

- To capture detialed traveling behaviors information, use `TraCI` API with following command:

```bash
python3 TraCI_Python_Version.py
```

##### 8. A Month Data Generation
- By randomly removing part of exiting route data and adding new routes, we can generate route data with detailed traveling behaviors in different days.
- Moreover, manually control the number of route data with different od-pairs or departure time can achieve different traveling behaviors.
- Limited to data size, it can be download from GoogleDrive.

## Model Training on Manual Allocation Data

##### 1. Edge Classification
- `Edge_Classification.ipynb` classifies al road segments into categories.
- `Route_generation.ipynb` generates the route data base on road segment in each categories as input to Sumo to generate Manual Allocation Data for light weight model training.

##### 2. SUMO Simulation
- `TraCI_Python_Adjusted.py` generates the Manual Allocation Data named TraCI_output_adjusted.csv by Sumo’s simulation procedure.
- Limited to data size, it can be download from GoogleDrive.

### 3. Model Training:
- `light_weight_model_training.ipynb` train light weight models based on the Manual Allocation Data, and models are stored in AutogluonModels/ag-20241207_012554 file.

## Traffic Simulation Prediction

##### 1. Convert Structure of Realistic Event Data as Input to RouteSys
- In `Manhattan_Data` file, `xml_data_2_txt 2.ipynb` generates `route.txt`, `query.txt`, and `time.txt` files as input to the routeSys.

##### 2. Model Catching
- In `Manhattan_Data` file, `Model_Catching.ipynb` file generated catched light weight models for routeSts.

##### 3. Traffic Simulation by RouteSys
- `Simulation_Prediction` is the C++ code for routeSys to accurately and efficiently simulate the future temporal information

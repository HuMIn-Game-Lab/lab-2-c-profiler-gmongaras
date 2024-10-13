import React, { useState } from 'react';
import BarChart from './components/BarChart';
import TimelineChart from './components/TimelineChart';
import './styles.css';

const App = () => {
    const [data, setData] = useState([]);
    const [filteredData, setFilteredData] = useState([]);
    const [sectionNames, setSectionNames] = useState([]);
    const [selectedSection, setSelectedSection] = useState('');
    const [logTransform, setLogTransform] = useState(false); // State to manage log transform
    const [sortMetric, setSortMetric] = useState('Avg Time'); // Sort by Average Time by default
    const [sortOrder, setSortOrder] = useState('asc'); // Sorting order
    const [filterThreshold, setFilterThreshold] = useState(0); // Filter threshold for time

    const handleFileUpload = (event) => {
        const file = event.target.files[0];
        if (file) {
            const reader = new FileReader();
            reader.onload = (e) => {
                const jsonData = JSON.parse(e.target.result).profiler;
                setData(jsonData);
                setFilteredData(jsonData);
                const sections = [...new Set(jsonData.map(d => d['Section Name']))];
                setSectionNames(sections);
            };
            reader.readAsText(file);
        }
    };

    const handleSectionFilter = (event) => {
        const section = event.target.value;
        setSelectedSection(section);
        filterAndSortData(section, sortMetric, sortOrder, filterThreshold);
    };

    const handleLogTransformToggle = (event) => {
        setLogTransform(event.target.checked); // Toggle log transform on or off
    };

    const handleSortMetricChange = (event) => {
        const metric = event.target.value;
        setSortMetric(metric);
        filterAndSortData(selectedSection, metric, sortOrder, filterThreshold);
    };

    const handleSortOrderChange = (event) => {
        const order = event.target.value;
        setSortOrder(order);
        filterAndSortData(selectedSection, sortMetric, order, filterThreshold);
    };

    const handleFilterThresholdChange = (event) => {
        const threshold = parseFloat(event.target.value);
        setFilterThreshold(threshold);
        filterAndSortData(selectedSection, sortMetric, sortOrder, threshold);
    };

    const filterAndSortData = (section, metric, order, threshold) => {
        let filtered = data;
        if (section) {
            filtered = filtered.filter(d => d['Section Name'] === section);
        }

        // Apply filtering based on threshold
        filtered = filtered.filter(d => +d[metric] >= threshold);

        // Apply sorting
        filtered.sort((a, b) => {
            const valA = +a[metric];
            const valB = +b[metric];
            if (order === 'asc') {
                return valA - valB;
            } else {
                return valB - valA;
            }
        });

        setFilteredData(filtered);
    };

    return (
        <div className="App">
            <h1>Profiler Output Visualization Tool</h1>

            {/* File Upload */}
            <div>
                <input type="file" accept=".json" onChange={handleFileUpload} />
            </div>

            {/* Log Transform Checkbox */}
            <div>
                <label>
                    <input type="checkbox" onChange={handleLogTransformToggle} />
                    Apply Log Transform to Average, Max, Min, and Total Time
                </label>
            </div>

            {/* Filter Section */}
            <div>
                <label htmlFor="filterSection">Filter by Section Name:</label>
                <select id="filterSection" value={selectedSection} onChange={handleSectionFilter}>
                    <option value="">All Sections</option>
                    {sectionNames.map((section, index) => (
                        <option key={index} value={section}>{section}</option>
                    ))}
                </select>
            </div>

            {/* Sort Metric */}
            <div>
                <label htmlFor="sortMetric">Sort By:</label>
                <select id="sortMetric" value={sortMetric} onChange={handleSortMetricChange}>
                    <option value="Avg Time">Average Time</option>
                    <option value="Max Time">Max Time</option>
                    <option value="Min Time">Min Time</option>
                    <option value="Total Time">Total Time</option>
                </select>
            </div>

            {/* Sort Order */}
            <div>
                <label htmlFor="sortOrder">Sort Order:</label>
                <select id="sortOrder" value={sortOrder} onChange={handleSortOrderChange}>
                    <option value="asc">Ascending</option>
                    <option value="desc">Descending</option>
                </select>
            </div>

            {/* Filter by Threshold */}
            <div>
                <label htmlFor="filterThreshold">Filter by Minimum Value:</label>
                <input
                    id="filterThreshold"
                    type="number"
                    value={filterThreshold}
                    onChange={handleFilterThresholdChange}
                    step="0.001"
                    min="0"
                />
            </div>

            {/* Display a message if no data meets the filter */}
            {data.length > 0 && filteredData.length === 0 ? (
                <p style={{ color: 'red', textAlign: 'center', fontSize: '1.2em' }}>
                    No data meets the filter criteria.
                </p>
            ) : (
                <div className="chart-container">
                    {filteredData.length > 0 && <BarChart data={filteredData} metric="Avg Time" metricLabel="Average Time" logTransform={logTransform} />}
                    {filteredData.length > 0 && <BarChart data={filteredData} metric="Max Time" metricLabel="Max Time" logTransform={logTransform} />}
                    {filteredData.length > 0 && <BarChart data={filteredData} metric="Min Time" metricLabel="Min Time" logTransform={logTransform} />}
                    {filteredData.length > 0 && <BarChart data={filteredData} metric="Total Time" metricLabel="Total Time" logTransform={logTransform} />}
                    {filteredData.length > 0 && <TimelineChart data={filteredData} />}
                </div>
            )}
        </div>
    );
};

export default App;

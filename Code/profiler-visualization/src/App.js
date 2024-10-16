import React, { useState } from 'react';
import BarChart from './components/BarChart';
import TimelineChart from './components/TimelineChart';
import './styles.css';

const App = () => {
    const [data, setData] = useState([]);
    const [filteredData, setFilteredData] = useState([]);
    const [sectionNames, setSectionNames] = useState([]);
    const [selectedSections, setSelectedSections] = useState([]);
    const [logTransform, setLogTransform] = useState(false);
    const [sortMetric, setSortMetric] = useState('Avg Time');
    const [sortOrder, setSortOrder] = useState('asc');
    const [filterThreshold, setFilterThreshold] = useState(0);
    const [showCheckboxes, setShowCheckboxes] = useState(true);  // Manage toggle state for checkboxes

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
                setSelectedSections(sections);
            };
            reader.readAsText(file);
        }
    };

    const handleSectionToggle = (section) => {
        const newSelectedSections = selectedSections.includes(section)
            ? selectedSections.filter(s => s !== section)
            : [...selectedSections, section];
        setSelectedSections(newSelectedSections);
        filterAndSortData(newSelectedSections, sortMetric, sortOrder, filterThreshold);
    };

    const handleLogTransformToggle = (event) => {
        setLogTransform(event.target.checked);
    };

    const handleSortMetricChange = (event) => {
        const metric = event.target.value;
        setSortMetric(metric);
        filterAndSortData(selectedSections, metric, sortOrder, filterThreshold);
    };

    const handleSortOrderChange = (event) => {
        const order = event.target.value;
        setSortOrder(order);
        filterAndSortData(selectedSections, sortMetric, order, filterThreshold);
    };

    const handleFilterThresholdChange = (event) => {
        const threshold = parseFloat(event.target.value);
        setFilterThreshold(threshold);
        filterAndSortData(selectedSections, sortMetric, sortOrder, threshold);
    };

    const filterAndSortData = (sections, metric, order, threshold) => {
        let filtered = data.filter(d => sections.includes(d['Section Name']));

        filtered = filtered.filter(d => +d[metric] >= threshold);
        filtered.sort((a, b) => (order === 'asc' ? +a[metric] - +b[metric] : +b[metric] - +a[metric]));

        setFilteredData(filtered);
    };

    const toggleCheckboxes = () => {
        setShowCheckboxes(!showCheckboxes);
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

            {/* Toggle Section Checkboxes */}
            <div className="checkbox-toggle-container">
                <button onClick={toggleCheckboxes} className="toggle-button">
                    {showCheckboxes ? '▼ Hide Sections' : '► Show Sections'}
                </button>
                {showCheckboxes && (
                    <div className="section-checkbox-container">
                        {sectionNames.map((section, index) => (
                            <label key={index}>
                                <input
                                    type="checkbox"
                                    checked={selectedSections.includes(section)}
                                    onChange={() => handleSectionToggle(section)}
                                />
                                {section}
                            </label>
                        ))}
                    </div>
                )}
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

            {/* Display Charts */}
            <div className="chart-container">
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Avg Time" metricLabel="Average Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Max Time" metricLabel="Max Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Min Time" metricLabel="Min Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <BarChart data={filteredData} metric="Total Time" metricLabel="Total Time" logTransform={logTransform} />}
                {filteredData.length > 0 && <TimelineChart data={filteredData} />}
            </div>
        </div>
    );
};

export default App;

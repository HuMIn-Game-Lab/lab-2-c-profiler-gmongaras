import React, { useRef, useEffect } from 'react';
import * as d3 from 'd3';

const BarChart = ({ data, metric, metricLabel, logTransform }) => {
    const svgRef = useRef();

    useEffect(() => {
        const svg = d3.select(svgRef.current);
        svg.selectAll('*').remove();  // Clear old elements to avoid overlap

        const margin = { top: 40, right: 30, bottom: 80, left: 70 };
        const width = 500 - margin.left - margin.right;
        const height = 400 - margin.top - margin.bottom;

        const x = d3.scaleBand()
            .domain(data.map(d => d['Section Name']))
            .range([0, width])
            .padding(0.1);

        // Conditionally apply log transform if logTransform is true
        const y = logTransform 
            ? d3.scaleLog()
                .base(10)
                .domain([1e-6, d3.max(data, d => +d[metric])])  // Log scale requires non-zero positive values
                .range([height, 0])
            : d3.scaleLinear()
                .domain([0, d3.max(data, d => +d[metric])])
                .range([height, 0]);

        const colorScale = d3.scaleOrdinal(d3.schemeSet3); // Use a vibrant color scheme

        const chart = svg.append("g")
            .attr("transform", `translate(${margin.left},${margin.top})`);

        // Add X axis
        chart.append("g")
            .attr("transform", `translate(0,${height})`)
            .call(d3.axisBottom(x))
            .selectAll("text")
            .attr("transform", "rotate(-45)")
            .style("text-anchor", "end");

        // Add Y axis
        chart.append("g")
            .call(d3.axisLeft(y).ticks(logTransform ? 10 : null, logTransform ? "~" : null));

        // Add X axis label
        svg.append("text")
            .attr("text-anchor", "middle")
            .attr("x", width / 2 + margin.left)
            .attr("y", height + margin.top + 50)
            .attr("fill", "#f5f5f5");

        // Add Y axis label
        svg.append("text")
            .attr("text-anchor", "middle")
            .attr("x", -height / 2)
            .attr("y", margin.left / 2 - 10)
            .attr("transform", "rotate(-90)")
            .text(metricLabel)
            .attr("fill", "#f5f5f5");

        // Draw colorful bars
        chart.selectAll(".bar")
            .data(data)
            .join("rect")
            .attr("class", "bar")
            .attr("x", d => x(d['Section Name']))
            .attr("y", d => y(+d[metric]))
            .attr("width", x.bandwidth())
            .attr("height", d => height - y(+d[metric]))
            .attr("fill", (d, i) => colorScale(i));  // Assign each bar a vibrant color

    }, [data, metric, metricLabel, logTransform]);  // Re-run effect if data or logTransform changes

    return (
        <svg ref={svgRef} width="500" height="500"></svg>
    );
};

export default BarChart;

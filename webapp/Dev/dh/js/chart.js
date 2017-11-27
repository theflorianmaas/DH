<script type="text/javascript">
	$(document).ready(function () {
		var source =
		{
			 datatype: "json",
			 datafields: [
				 { name: 'Date', type: 'date'},
				 { name: 'Value'},
				 { name: 'Sensor'}
			],
			document.write("'url: ''php/getdatachartsensor.php?id='" + id + "'"); 
		};

	   var dataAdapter = new $.jqx.dataAdapter(source,
		{
			autoBind: true,
			async: false,
			downloadComplete: function () { },
			loadComplete: function () { },
			loadError: function () { }
		});

	 // prepare jqxChart settings
		var settings = {
			title: "Orders by Date",
			showLegend: true,
			padding: { left: 5, top: 5, right: 5, bottom: 5 },
			titlePadding: { left: 90, top: 0, right: 0, bottom: 10 },
			source: dataAdapter,
			categoryAxis:
				{
					text: 'Category Axis',
					textRotationAngle: 0,
					dataField: 'Date',
					formatFunction: function (value) {
						return $.jqx.dataFormat.formatdate(value, 'HH.mi');
					},
					showTickMarks: true,
					tickMarksInterval: Math.round(dataAdapter.records.length / 6),
					tickMarksColor: '#888888',
					unitInterval: Math.round(dataAdapter.records.length / 6),
					showGridLines: true,
					gridLinesInterval: Math.round(dataAdapter.records.length / 3),
					gridLinesColor: '#888888',
					axisSize: 'auto'
				},
			colorScheme: 'scheme05',
			seriesGroups:
				[
					{
						type: 'line',
						valueAxis:
						{
							displayValueAxis: true,
							description: 'Sensor',
							//descriptionClass: 'css-class-name',
							axisSize: 'auto',
							tickMarksColor: '#888888',
							unitInterval: 20,
							minValue: 0,
							maxValue: 100
						},
						series: [
								{ dataField: 'Value', displayText: 'Value' }
						  ]
					}
				]
		};

		// setup the chart
		$('#jqxChart').jqxChart(settings);
	});
</script>
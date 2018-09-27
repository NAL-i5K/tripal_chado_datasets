<?php 
$data = $variables['data'];
?>

<p><b>Organism:</b> <?php print $data[0]->organism; ?></p> 
<p><b>Submitter Name:</b> <?php print $data[0]->name; ?> </p> 
<p><b>Email Address:</b> <?php print $data[0]->email; ?> </p>
<!-- <p><b>Affiliation:</b> <?php print $data[0]->affiliation; ?></p> -->
<p><b>Program:</b> <?php print $data[0]->program; ?></p>
<p><b>Version:</b> <?php print $data[0]->version; ?></p>
<p><b>Additional Information:</b> <?php print $data[0]->additional_info; ?></p>
<p><b>Methods Citaion (DOI):</b> <?php print $data[0]->other_methods; ?></p>
<p><b>Dataset name:</b> <?php print $data[0]->dataset_name; ?></p>
<p><b>Dataset Version:</b> <?php print $data[0]->dataset_version; ?></p>
<p><b>Should we make this file available for download in our Data Downloads section?:</b> <?php print $data[0]->is_download; ?></p>
<p><b>Is the dataset published?:</b> <?php print $data[0]->dataset_is_publish; ?></p>
<?php
if ($data[0]->dataset_is_publish == 'Yes') {
?>
<p><b>If yes:</b> <?php print $data[0]->dataset_publish_field_data; ?></p>
<?php
} else {
?>
<p><b>If no:</b> <?php print $data[0]->dataset_publish_field_data; ?></p>
<?php
}
?>
<br>

<h4>Mapped dataset:</h4>
<p><b>Geographic location (latitude and longitude):</b> <?php print $data[0]->mapped_dataset_geo_location; ?></p>
<p><b>Tissues/Life stage included:</b> <?php print $data[0]->mapped_dataset_tissues_located; ?></p>
<?php
if ($data[0]->assembly_gender == 'NA') {
	?>
<p><b>Other (sex):</b> <?php print $data[0]->other_gender; ?></p>
	<?php
}
?>
<p><b>Sequencing method (e.g. Illumina Hi-Seq 200 bp):</b> <?php print $data[0]->sequence_platform; ?></p>
<p><b>Descriptive track name for JBrowse and Apollo:</b> <?php print $data[0]->mapped_data_source_url; ?></p>
<p><b>NCBI SRA accession number(s):</b> <?php print $data[0]->assembly_accession; ?></p>

<?php if(!empty($data[0]->SHA512)) { ?>
<b>SHA512:</b> <?php print $data[0]->sha512; ?><br><br>
<?php } ?>

<b>File Information:</b><br>
<?php
  $file = explode(',', $data[0]->filename);
  foreach($file as $key => $name) {
  	if(!empty($name))
  		echo $name . "<br>";
  }
?>
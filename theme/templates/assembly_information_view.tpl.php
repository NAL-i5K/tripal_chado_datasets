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
<p><b>Methods Citaion (DOI):</b> <?php print $data[0]->methods_citation; ?></p>
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

<h4>Genome Assembly Information:</h4>
<b>Project Background</b><br><br>
<p><b>Project description to display in your organism page:</b> <?php print $data[0]->description; ?><br></p>
<p><b>is_curate_assembly:</b> <?php print $data[0]->is_curate_assembly; ?></p><br>
<?php if($data[0]->is_curate_assembly == 'Yes') { ?>
<p><b>is_same:</b> <?php print $is_same = ($data[0]->is_same == 1)?'Yes':'No'; ?></p><br>
<p><b>manual_curation_name:</b>  <?php print $data[0]->manual_curation_name; ?></p><br>
<p><b>manual_curation_email:</b> <?php print $data[0]->manual_curation_email; ?></p><br>
<p><b>need_assistance:</b> <?php print $data[0]->need_assistance; ?></p><br>

<?php if(!empty($data[0]->reason)) { ?>
<p><b>reason:</b> <?php print $data[0]->reason; ?></p><br>
<?php } ?>
<?php 
if (($data[0]->time_from == '0000-00-00' && $data[0]->time_to == '0000-00-00') || ($data[0]->time_from == '0' && $data[0]->time_to == '0')) {
?>
<p><b>Specify curation time frame:</b> There is no set timeframe for curation.</p><br>
<?php } else {?>
<p><b>Start date:</b> <?php print date('M d Y', strtotime($data[0]->time_from)); ?></p><br>
<p><b>End date:</b> <?php print date('M d Y', strtotime($data[0]->time_to)); ?></p><br>
<?php }
 } ?>
<p><b>Geographic location (latitude and longitude):</b> <?php print $data[0]->assembly_geo_location; ?></p>
<p><b>Tissues/Life stage included:</b> <?php print $data[0]->assembly_tissues_located; ?></p>
<p><b>Sex:</b> <?php print $data[0]->assembly_gender; ?></p>
<?php
if ($data[0]->assembly_gender == 'NA') {
	?>
<p><b>Other (sex):</b> <?php print $data[0]->other_gender; ?></p>

	<?php
}
?>

<p><b>Strain:</b> <?php print $data[0]->data_source_strain; ?></p>
<p><b>Other notes:</b> <?php print $data[0]->data_source_notes; ?></p>
<p><b>Sequencing method (e.g. Illumina Hi-Seq 200 bp):</b> <?php print $data[0]->data_source_seqplatform; ?></p>
<p><b>NCBI/INSDC Genome Assembly accession:</b> <?php print $data[0]->assembly_accession; ?></p>
<p><b>Other notes:</b> <?php print $data[0]->additional_other_notes; ?></p>

<?php if(!empty($data[0]->sha512)) { ?>
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

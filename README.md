Module status **under development, not suitable for deployment**.

The datasets module provides forms for users to submit data.  This includes: 

* creating an organism, and waiting for admin approval before its created
* creating an analysis, and waiting for admin approval before its created 
  - module does not currently create analyses
  

To see this module in action, pleas visit https://i5k.nal.usda.gov/datasets/submit-a-dataset 

# Set up

### Patch the captcha module
1. Applied patch to captcha.module file due to erro "CAPTCHA session reuse attack detected"

https://www.drupal.org/files/issues/1395184_31.patch


### Set the admin email variable


At the start of the module file, you modify this line of code to point to `define('FROM_ADDRESS', '"NAL-i5k"<i5k@ars.usda.gov>');`

### More

Steps: for New Organism
1. Request new organism If its not there in dropdown
   To request go to url: http://domain_url/datasets/request-project
2. Once the user submitted the request organism, an email goes to administrator.
3. Administrator checks and sees if the organism is worth to approve/reject.
4. An email goes to the requested user about the organism approval if it is approved. If rejected he wont   receives and email.

## Dependencies
the below modules will be installed when enabling datasets:

1. Date module
2. honeypot module



TODO: handle this on install
>create a "datasets" folder for organism image under sites/default/files/
TODO: handle this on install
4. create db_tables in databse.


# Admin usage

Currently, if running a Tripal 3 site, organism records will be inserted into Chado after approving a request at `admin/structure/datasets` in the "Request Project List" tab.  They will not be available to users until they are **published**.  Visit `admin -> Content Types -> Tripal Content types -> Publish` after approving an organism to publish it.
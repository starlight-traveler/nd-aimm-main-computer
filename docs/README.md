# **NDAIMM Main Computer Documentation**

***Notre Dame Artifical Intelligence & Machine Learning and Modeling Team***

This is the home of all the documentation for the Main LPV computer. Please review the below and go to ***Getting Started*** in order to access the programming suite.

Inspired by literate programming, maintained by the Notre Dame Artifical Intelligence team, and built with a focus on trunk-based development and industry best practices, this NDAIMM project contains comprehensive documentation, automated testing, CI/CD, and secure coding practices. Some key things to review project management wise:

- [**README**](README), [**CODE_OF_CONDUCT**](docs/CODE_OF_CONDUCT.md), [**CONTRIBUTING**](docs/CONTRIBUTING.md)

  > README files are important and often neglected. The files should inform anyone about the first steps to use, learn and contribute to your project.
  >
- [**CITATION.cff**](CITATION.cff)

  > Embracing [CFF](https://citation-file-format.github.io) aligns with best practices for reproducible research and software development. By adhering to established standards for documenting project dependencies and citations, NDAIMM demonstrates our commitment to quality, transparency, and integrity in our work.
  >
- [**LICENSE**](LICENSE)

  > The LICENSE is a pivotal document that delineates the permissions and restrictions for utilizing the contents of this repository. In the absence of a license, no rights are granted to use, modify, or distribute the code. This repository is licensed under the** **[**MIT License**](), ensuring both simplicity and permissiveness in its usage.
  >
- **docs/**

  > Documentation is a crucial part to the overall readability and clarity in NDAIMM's work. Doxygen and GraphViz documentation is generated in the docs/ folder during CI/CD and can be accessed through this official documentation page.
  >
- [**docs/bibliography.bib**](/docs/bibliography.bib)

  > This bibliography will have citations to all of NDAIMM's outside work pulled in for this research project. This bibliography uses the [BibTeX](https://www.bibtex.org/Format/) format. See also [Citations and bibliographies](https://jupyterbook.org/en/stable/content/citations.html).
  >
- **src/**

  > All applications of programming done in the CXX standard will occur in the src/ directory and the exectuable will build from there. Under no circumstance should code be elevated out of the **src/** Usage
  >

```{important}

Your pull request to merge code will be denied if anything is elevated outside of the directory, you must discuss with the project leads beforehand.
```


### Code of Conduct

The Notre Dame Artifical Intelligence Team maintains a [Code of Conduct](docs/CODE_OF_CONDUCT.md) to ensure an inclusive and respectful environment for everyone. Please adhere to it in all interactions within our community.

## License

The Notre Dame Artifical Intelligence Team's code is licensed under the** ****MIT License**. This project is fully open-source and every effort should be put forward for the FOSS community.
